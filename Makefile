# C++ compiler
CXX = clang++
CXXFLAGS = -std=c++17

# source code: all files with .cpp extension
SRC = $(wildcard src/*.cpp)

# object files: take source file stems and replace extension with .o
OBJ = $(SRC:src/%.cpp=build/%.o)

# name of executable program
TARGET = depgen

# there aren't actually any targets called `all` or `clean`
.PHONY: all clean

# first rule is carried out by default
all: $(TARGET)

# link all object files to form executable
$(TARGET): $(OBJ)
	$(CXX) $^ -o $@

# given a single source file, compile to an object file (built-in)
build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# add dependencies here, e.g. main.o: header.h
build/depgen.o: src/constants.h src/depgen.h src/utils.h
build/main.o: src/depgen.h src/utils.h
build/utils.o: src/utils.h

# remove "auxilliary" files	
clean:
	rm $(TARGET) $(OBJ)
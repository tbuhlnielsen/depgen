# Depgen

Generates dependencies to include in makefiles for small/medium C++ projects.


## Usage

To build the executable program, `depgen`, clone or download this project and do
```
$ mkdir build
$ make
```
from its root[^1]. On running the executable from the root of any C++ project, it
will scan the project's source files for `#include` commands and (try to)
write a file -- called `.depend` by default -- consisting of a list of dependencies
suitable for inclusion in a makefile.


## Options

1. `-o <name>` Set the name of the output file.

2. `-r` Search for source files recursively (rather than only in the project's 
root directory).

3. `-I <directory>` Specify a directory in which to search for a header file 
not found in the parent directory of the source file which includes it.


## Warning

The program currently assumes that all object files (resulting from compilation of
the project's source files) will be in a directory called `build` in the 
project's root.

It might be possible to lift this restriction by giving users the option to specify
the locations of object files with an input file.


## To do

1. Add an option to include additional extensions of source files (other than 
.cpp, .cxx, .C, and .cc).

2. Give warnings on circular or duplicate dependencies.

3. Test `trim(fs::path)` on a path not in the project root.


[^1]: Only tested on macOS Catalina with clang and C++17.
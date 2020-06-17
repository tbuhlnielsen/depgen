/*
    depgen.h - Definition of the class Depgen, which is responsible for 
    scanning source files for #include commands and writing the resulting 
    dependencies to an output file.
*/

#ifndef DEPGEN_H
#define DEPGEN_H

#include <fstream>
#include <map>

#include "utils.h"

namespace fs = std::filesystem;

// ----------------

/* To form the dependency graph for a project, each source file is 
associated with the files it includes. */
typedef std::map<fs::path, std::vector<fs::path>> depend_graph;

/* Processes the command line arguments described by @argc and @argv 
and constructs and runs a Depgen using the resulting Options. */
void make_depend(int argc, char* argv[]);

/* A Depgen scans C++ source files provided to it for #include commands
and write the resulting dependencies to an output file. */
class Depgen
{
    public:
        /* If a file, f, in @srcs includes a file, g, which cannot be found in
        f's parent directory then g will be searched for in @incl_dirs. */
        Depgen(std::vector<fs::path> srcs, std::vector<fs::path> incl_dirs);

        /* Performs a breadth-first search to find the dependencies of 
        Depgen::sources by recursively scanning for #include commands. */
        void scan();

        /* Writes Depgen::dependencies to @target in a format suitable for 
        use in a makefile. */
        void write_to(std::ofstream& target);

    private:
        std::vector<fs::path> sources;          // Set by constructor.
        std::vector<fs::path> include_dirs;     // Set by constructor.

        depend_graph dependencies;              // Modified by scan().

        /* Returns (the paths of) the files directly included by @p. */
        std::vector<fs::path> children(fs::path p);
};

// ----------------

// Helper functions for Depgen::scan().

/* Returns (the paths of) all files included by @p. */
std::vector<fs::path> included_files(fs::path p);

/* Reads characters from @ist and returns true iff something of the form
`#include <filename>` or `#include "filename"` is read (unless, in the 
first case, `filename` is a standard library header). In this case, 
`filename` is read into @header. */
bool read_include_cmd(std::istream& ist, std::string& header);

/* Reads characters from @ist until a one that is not a letter, a digit, an 
underscore, a dot, or a forward slash is seen. Returns a (possibly empty) 
string of the read characters. */
std::string read_word(std::istream& ist);

// ----------------

// Functions for finding source files to pass to a Depgen.

/* Returns the files in K::project_root (and its sub-directories, if 
@recursive == true) with an extension in @exts. */
std::vector<fs::path> get_sources(bool recursive, const std::vector<std::string>& exts);

/* Removes K::project_root's path from the start of @p. Throws an error
if @p is not in (a sub-directory of) K::project_root. */
fs::path trim(fs::path p);

#endif 
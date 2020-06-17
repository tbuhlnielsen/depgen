/*
    utils.h - Declarations of tools for handling command line arguments 
    (and declarations of some utility functions).
*/

#ifndef UTILS_H
#define UTILS_H

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// ----------------

/* Settings for the program which a user can specify from the command line. */
struct Options
{
    /* Name of the output file in which dependencies are written. */
    std::string target {".depend"};

    /* Root directory in which source files to be scanned for dependencies are 
    searched for. */
    fs::path root = fs::current_path();

    /* Should source files be searched for in root's sub-directories? */
    bool recursive = false;

    /* The extensions of source files which will be scanned for #include 
    commands. */
    std::vector<std::string> src_exts {".cpp", ".cxx", ".C", ".cc"};

    std::vector<fs::path> include_dirs;
};

/* Reads arguments passed to the program from the command line and returns
the corresponding Options. */
Options process_cmd_line_args(int argc, char* argv[]);

// ----------------

// Use template?

bool contains(std::vector<std::string>, std::string);

bool contains(std::vector<fs::path>, fs::path);

// ----------------

#endif
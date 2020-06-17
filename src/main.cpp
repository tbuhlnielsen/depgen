/*
    This program builds a dependency graph for a small/medium C++ project by
    scanning source files for #include commands. It then writes the dependencies
    to an outupt file in a format suitable for use in a makefile.
*/

#include <stdexcept>

#include "depgen.h"

// ----------------

int main(int argc, char* argv[])
{
    try
    {
        make_depend(argc, argv);

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown error!\n";
        return 2;
    }
}
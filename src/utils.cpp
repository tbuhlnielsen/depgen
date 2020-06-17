#include <algorithm>
#include <stdexcept>

#include "utils.h"

// ----------------

Options process_cmd_line_args(int argc, char* argv[])
{
    Options opts;

    // Define flags to record which options have been set?
    // (To prevent users setting some options twice.)

    for (int i = 1; i < argc; ++i)
    {
        if (std::strncmp(argv[i], "-r", 2) == 0)
            opts.recursive = true;

        else if (std::strncmp(argv[i], "-I", 2) == 0)
        {
            ++i;
            if (i < argc)
            {
                auto dir = fs::path(argv[i]);

                if (!fs::exists(dir))
                    throw std::runtime_error("directory \'" + std::string(argv[i]) + "\' not found");

                if (!fs::is_directory(dir))
                    throw std::runtime_error("\'" + std::string(argv[i]) + "\' not a directory");

                opts.include_dirs.push_back(opts.root / dir);
            }
            else
                throw std::runtime_error("no directory supplied to -I");
        }

        else if (std::strncmp(argv[i], "-o", 2) == 0)
        {
            ++i;
            if (i < argc)
                opts.target = argv[i];
            else
                throw std::runtime_error("no output file supplied to -o");
        }

        else 
            throw std::runtime_error("unrecognised argument: " + std::string(argv[i]));
    }

    return opts;
}

// ----------------

bool contains(std::vector<std::string> v, std::string s)
{
    auto result = std::find(v.begin(), v.end(), s);
    
    return (result != v.end());
}

bool contains(std::vector<fs::path> v, fs::path p)
{
    auto result = std::find(v.begin(), v.end(), p);

    return (result != v.end());
}

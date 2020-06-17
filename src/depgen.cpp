#include <queue>
#include <sstream>
#include <stdexcept>

#include "constants.h"
#include "depgen.h"

// ----------------

void make_depend(int argc, char* argv[])
{
    Options opts = process_cmd_line_args(argc, argv);

    std::ofstream output_file {opts.target};
    if (!output_file)
        throw std::runtime_error("failed to open " + opts.target);

    std::vector<fs::path> sources = get_sources(opts.recursive, opts.src_exts);

    Depgen dep_gen {sources, opts.include_dirs};
    dep_gen.scan();
    dep_gen.write_to(output_file);
}

// ----------------

Depgen::Depgen(std::vector<fs::path> srcs, std::vector<fs::path> incl_dirs)
    :sources{srcs}, include_dirs{incl_dirs}
{
}

// ----------------

// Functions for scanning source files.

void Depgen::scan()
{
    for (const auto& source : sources)
    {
        std::vector<fs::path> detected {source};

        std::queue<fs::path> scan_q;
        scan_q.push(source);

        // std::cerr << "Scanning " << trim(source.string()) << '\n';

        while (!scan_q.empty())
        {
            fs::path p = scan_q.front();
            scan_q.pop();

            for (const fs::path& file : children(p))
            {
                if (!contains(detected, file))
                {
                    detected.push_back(file);
                    scan_q.push(file);
                    dependencies[source].push_back(file);
                }
            }
        }
    }
}

bool found_in(fs::path dir, fs::path& file)
{
    fs::path test = dir / file;

    // std::cerr << "Trying: " << trim(test.string()) << '\n';

    if (fs::exists(test))
    {
        file = test;
        return true;
    }

    return false;
}

std::vector<fs::path> Depgen::children(fs::path p)
{
    std::vector<fs::path> successors; // will be returned

    for (fs::path& file : included_files(p))
    {
        // std::cerr << "Included: " << file.string() << '\n';

        bool found = false;

        if (file.is_absolute() || found_in(p.parent_path(), file))
            found = true;
        else
        {
            for (const fs::path& dir : include_dirs)
            {
                if (found_in(dir, file))
                {
                    found = true;
                    break;
                }
            }
        }

        if (!found)
            throw std::runtime_error("file \'" + file.string() + "\' not found");

        // Check if a header is included twice in one file.
        if (found && !contains(successors, file))
            successors.push_back(file);
    }

    return successors;
}

std::vector<fs::path> included_files(fs::path p)
{
    std::ifstream file {p};
    if (!file)
        throw std::runtime_error("failed to open " + p.string());

    std::vector<fs::path> files; // will be returned

    for (std::string line; std::getline(file, line);)
    {
        std::istringstream input {line};

        std::string header;
        if (read_include_cmd(input, header))
            files.push_back(fs::path(header));
    }

    return files;
}

bool read_include_cmd(std::istream& ist, std::string& header)
{
    char intro;
    ist >> intro;
    if (intro != '#')
        return false;

    ist >> std::ws;
    std::string cmd = read_word(ist);
    if (cmd != "include")
        return false;

    char delim;
    ist >> delim;
    if (delim != '\"' && delim != '<')
        return false;

    header = read_word(ist);

    // Ignore dependencies on standard library headers.
    if (delim == '<' && contains(K::std_headers, header))
        return false;

    // Assume right delimiter ('\"' or '>') is present.

    return true;
}

std::string read_word(std::istream& ist)
{
    std::string word;

    for (char ch; ist.get(ch);)
    {
        if (!isalnum(ch) && ch != '.' && ch != '/' && ch != '_')
        {
            ist.unget();
            return word;
        }

        word += ch;
    }

    return word;
}

// ----------------

void Depgen::write_to(std::ofstream& target)
{
    for (const auto& node : dependencies)
    {
        fs::path src = node.first;

        // What if object files aren't all in a directory called build???
        fs::path obj = src.stem();

        target << "build/" << obj.string() << ".o: ";

        std::vector<fs::path> deps = dependencies[src];

        for (const auto& file : deps)
        {
            if (file != deps.front())
                target << std::string(obj.string().length() + 10, ' ');

            target << trim(file).string();

            if (file != deps.back())
                target << " \\\n";
        }

        target << '\n';
    }
}

// ----------------

// Functions for finding source files to pass to a Depgen.

std::vector<fs::path> get_sources(bool recursive, const std::vector<std::string>& exts)
{
    std::vector<fs::path> sources;

    if (recursive)
    {
        for (const auto& file : fs::recursive_directory_iterator(K::project_root))
        {
            std::string s = file.path().extension();

            if (contains(exts, s))
                sources.push_back(file.path());
        }
    }
    else
    {
        for (const auto& file : fs::directory_iterator(K::project_root))
        {
            std::string s = file.path().extension();

            if (contains(exts, s))
                sources.push_back(file.path());
        }
    }

    return sources;
}

fs::path trim(fs::path p)
{
    fs::path trimmed = p.filename();

    while (p.parent_path() != K::project_root)
    {
        p = p.parent_path();

        if (p == p.parent_path())
            throw std::runtime_error("file not found in project root");

        trimmed = p.stem() / trimmed;
    }

    return trimmed;
}
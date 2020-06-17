#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// ----------------

namespace K {

const fs::path project_root = fs::current_path();

// C++17 standard library headers, from cppreference
const std::vector<std::string> std_headers {
    "algorithm",
    "any",
    "array",
    "atomic",
    "bitset",
    "cassert",
    "cctype",
    "cerrno",
    "cwtype",
    "cfenv",
    "cfloat",
    "charconv",
    "chrono",
    "cinttypes",
    "climits",
    "clocale",
    "cmath",
    "complex",
    "condition_variable",
    "csetjmp",
    "csignal",
    "cstdarg",
    "cstddef",
    "cstdint",
    "cstdio",
    "cstdlib",
    "cstring",
    "ctime",
    "cuchar",
    "cwchar",
    "deque",
    "excecution",
    "exception",
    "filesystem",
    "forward_list",
    "fstream",
    "functional",
    "future",
    "initializer_list",
    "iomanip",
    "ios",
    "iosfwd",
    "iostream",
    "istream",
    "iterator",
    "list",
    "locale",
    "limits",
    "map",
    "memory",
    "memory_resource",
    "mutex",
    "new",
    "numeric",
    "optional",
    "ostream",
    "queue",
    "random",
    "ratio",
    "regex",
    "scoped_allocator",
    "set",
    "shared_mutex",
    "sstream",
    "stack",
    "stdexcept",
    "streambuf",
    "string",
    "string_view",
    "system_error",
    "thread",
    "tuple",
    "typeindex",
    "typeinfo",
    "type_traits",
    "unordered_map",
    "unordered_set",
    "utility",
    "valarray",
    "vector"
};

} // K

#endif
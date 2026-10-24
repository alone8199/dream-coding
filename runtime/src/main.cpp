// main.cpp
// dc 的入口。把命令行参数交给运行器，然后醒来。

#include <iostream>
#include <string>
#include <vector>

#include "dc/runner.hpp"

int main(int argc, char** argv) {
    std::vector<std::string> args;
    args.reserve(argc > 1 ? static_cast<std::size_t>(argc - 1) : 0);
    for (int i = 1; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }

    return dc::run_command(args, std::cout, std::cerr);
}

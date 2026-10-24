# 33 The Build System: CMake and Cross-Platform

`dc` uses CMake as its build system. CMake is a tool that generates native build files, so the same configuration works on Linux, macOS and Windows. For a command-line tool that needs to support multiple platforms, this matters a great deal.

The project root contains a `CMakeLists.txt`, which defines three main targets. The first is `dc_core`, a static library containing the lexer, parser, evaluator, built-in functions, the standard library and the command-line logic. The second is `dc`, the executable, which contains only the entry function and links to `dc_core`. The third is the test programs, which also link to `dc_core` and verify the behavior of each part.

Placing the core logic in a static library lets the tests and the executable share the same code. The tests do not need to copy the implementation, nor call the command line indirectly. They can create an interpreter directly, run code, and check the results. This structure makes tests faster and more precise.

Compiler options are set at configuration time. On GCC and Clang, the project enables `-Wall`, `-Wextra` and `-Wpedantic`, which check for common errors and non-standard code. On MSVC, the project uses `/W4` and `/utf-8`. The latter ensures that Chinese characters in source code are handled correctly, which matters especially for `dc`, because the project's comments and some test data contain Chinese.

The C++ standard is set to C++17. This standard provides modern features such as `std::variant` and `std::filesystem`, which are used for value representation and file operations. CMake also turns off compiler extension mode, so the code behaves consistently across different compilers.

The installation rule is simple: the `dc` executable is installed into the `bin` directory. The installation step in CI installs it into the `dist` directory of the workspace, then runs smoke tests with the `dc` in that directory, to confirm that the installed program works correctly.

CTest integration is part of the build system. The project enables test support with `include(CTest)`, and when `BUILD_TESTING` is on, it registers two test programs. Developers can run all tests at once with `ctest`, or run just one.

The basic build workflow is: generate the build directory with `cmake -S . -B build`, compile with `cmake --build build`, and run the tests with `ctest --test-dir build`. These three commands make up the standard workflow of this project, and they are also written into the CI configuration.

In short, CMake keeps the build of `dc` simple and portable. Its configuration is short and clear, and its rules are explicit, so any developer can get a working program from the source in a few minutes.

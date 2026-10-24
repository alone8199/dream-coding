cmake_minimum_required(VERSION 3.16)

project(dc
    VERSION 0.3.0
    DESCRIPTION "Dream Coding: a Python-like interpreter that runs in the subconscious"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 解释器核心：词法、语法、值、求值器、内置函数、标准库、命令行逻辑。
add_library(dc_core STATIC
    runtime/src/dc/lexer.cpp
    runtime/src/dc/parser.cpp
    runtime/src/dc/value.cpp
    runtime/src/dc/interpreter.cpp
    runtime/src/dc/builtins.cpp
    runtime/src/dc/stdlib.cpp
    runtime/src/dc/runner.cpp)
target_include_directories(dc_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/runtime/src)

# 命令行程序。
add_executable(dc runtime/src/main.cpp)
target_link_libraries(dc PRIVATE dc_core)

install(TARGETS dc RUNTIME DESTINATION bin)

# include(CTest) 会提供 BUILD_TESTING 选项，默认开启。
include(CTest)
if(BUILD_TESTING)
    add_executable(dc_tests runtime/tests/runner_test.cpp)
    target_link_libraries(dc_tests PRIVATE dc_core)
    add_test(NAME dc_tests COMMAND dc_tests)

    add_executable(dc_interp_tests runtime/tests/interp_test.cpp)
    target_link_libraries(dc_interp_tests PRIVATE dc_core)
    add_test(NAME dc_interp_tests COMMAND dc_interp_tests)
endif()

cmake_minimum_required(VERSION 3.16)

project(dc
    VERSION 0.2.0
    DESCRIPTION "Dream Coding runner: compile in subconscious, run in nonexistent"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 运行器核心，供可执行文件与测试共用。
add_library(dc_core STATIC src/dc/runner.cpp)
target_include_directories(dc_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# 命令行程序。
add_executable(dc src/main.cpp)
target_link_libraries(dc PRIVATE dc_core)

install(TARGETS dc RUNTIME DESTINATION bin)

# include(CTest) 会提供 BUILD_TESTING 选项，默认开启。
include(CTest)
if(BUILD_TESTING)
    add_executable(dc_tests tests/runner_test.cpp)
    target_link_libraries(dc_tests PRIVATE dc_core)
    add_test(NAME dc_tests COMMAND dc_tests)
endif()

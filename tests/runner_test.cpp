// tests/runner_test.cpp
// dc 运行器的单元测试。没有测试框架，只用最朴素的断言。
//
// 在梦里，这些测试全部通过。醒来后请运行 ctest 确认。

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "dc/runner.hpp"

namespace {

int failures = 0;

#define EXPECT_TRUE(cond)                                                          \
    do {                                                                           \
        if (!(cond)) {                                                             \
            ++failures;                                                            \
            std::cerr << __FILE__ << ":" << __LINE__ << ": EXPECT_TRUE(" #cond ")\n"; \
        }                                                                          \
    } while (0)

#define EXPECT_EQ(actual, expected)                                                 \
    do {                                                                            \
        const auto _actual = (actual);                                              \
        const auto _expected = (expected);                                          \
        if (!(_actual == _expected)) {                                              \
            ++failures;                                                             \
            std::cerr << __FILE__ << ":" << __LINE__ << ": EXPECT_EQ(" #actual ", " \
                      << #expected ") got " << _actual << "\n";                     \
        }                                                                           \
    } while (0)

struct Result {
    int code;
    std::string out;
    std::string err;
};

Result invoke(const std::vector<std::string>& args) {
    std::ostringstream out;
    std::ostringstream err;
    const int code = dc::run_command(args, out, err);
    return {code, out.str(), err.str()};
}

bool contains(const std::string& haystack, const std::string& needle) {
    return haystack.find(needle) != std::string::npos;
}

void test_analyze_balanced() {
    const auto report = dc::analyze("dream X { fn a() { } }");
    EXPECT_TRUE(report.balanced());
    EXPECT_EQ(report.lines, static_cast<std::size_t>(1));
}

void test_analyze_unclosed() {
    const auto report = dc::analyze("{ {");
    EXPECT_TRUE(!report.balanced());
    EXPECT_EQ(report.final_depth, 2L);
}

void test_analyze_extra_close() {
    const auto report = dc::analyze("}");
    EXPECT_TRUE(!report.balanced());
    EXPECT_EQ(report.depth_errors, static_cast<std::size_t>(1));
}

void test_analyze_ignores_braces_in_strings() {
    EXPECT_TRUE(dc::analyze("let s = \"{\";").balanced());
    EXPECT_TRUE(dc::analyze("let s = \"\\\"}\";").balanced());
}

void test_analyze_ignores_braces_in_comments() {
    const auto report = dc::analyze("// }\n{}");
    EXPECT_TRUE(report.balanced());
    EXPECT_EQ(report.comments, static_cast<std::size_t>(1));
}

void test_extension() {
    EXPECT_TRUE(dc::has_dc_extension("main.dc"));
    EXPECT_TRUE(dc::has_dc_extension("MAIN.DC"));
    EXPECT_TRUE(dc::has_dc_extension("src/stages/run.dc"));
    EXPECT_TRUE(!dc::has_dc_extension("main.dream"));
    EXPECT_TRUE(!dc::has_dc_extension(".dc"));
    EXPECT_TRUE(!dc::has_dc_extension("dc"));
}

void test_no_args_is_usage_error() {
    const auto r = invoke({});
    EXPECT_EQ(r.code, static_cast<int>(dc::kExitUsage));
    EXPECT_TRUE(contains(r.err, "usage"));
}

void test_help() {
    const auto r = invoke({"help"});
    EXPECT_EQ(r.code, static_cast<int>(dc::kExitOk));
    EXPECT_TRUE(contains(r.out, "commands"));
}

void test_version() {
    const auto r = invoke({"version"});
    EXPECT_EQ(r.code, static_cast<int>(dc::kExitOk));
    EXPECT_TRUE(contains(r.out, dc::kVersion));
}

void test_sleep_default() {
    const auto r = invoke({"sleep"});
    EXPECT_EQ(r.code, static_cast<int>(dc::kExitOk));
    EXPECT_TRUE(contains(r.out, "REM"));
}

void test_sleep_too_short() {
    const auto r = invoke({"sleep", "4"});
    EXPECT_EQ(r.code, static_cast<int>(dc::kExitSleepTooShort));
    EXPECT_TRUE(contains(r.err, "TimeoutError"));
}

void test_sleep_exact_minimum() {
    const auto r = invoke({"sleep", "7"});
    EXPECT_EQ(r.code, static_cast<int>(dc::kExitOk));
}

void test_sleep_bad_number() {
    const auto r = invoke({"sleep", "abc"});
    EXPECT_EQ(r.code, static_cast<int>(dc::kExitUsage));
}

void test_wake_dumps() {
    const auto r = invoke({"wake"});
    EXPECT_EQ(r.code, static_cast<int>(dc::kExitOk));
    EXPECT_TRUE(contains(r.out, dc::kDumpMessage));
}

void test_run_missing_file() {
    const auto r = invoke({"run", "this-dream-does-not-exist.dc"});
    EXPECT_EQ(r.code, static_cast<int>(dc::kExitFileError));
    EXPECT_TRUE(contains(r.err, "FileNotFound"));
}

void test_run_wrong_extension() {
    const auto r = invoke({"run", "notes.txt"});
    EXPECT_EQ(r.code, static_cast<int>(dc::kExitUsage));
}

void test_unknown_command() {
    const auto r = invoke({"deploy"});
    EXPECT_EQ(r.code, static_cast<int>(dc::kExitUsage));
    EXPECT_TRUE(contains(r.err, "unknown command"));
}

}  // namespace

int main() {
    test_analyze_balanced();
    test_analyze_unclosed();
    test_analyze_extra_close();
    test_analyze_ignores_braces_in_strings();
    test_analyze_ignores_braces_in_comments();
    test_extension();
    test_no_args_is_usage_error();
    test_help();
    test_version();
    test_sleep_default();
    test_sleep_too_short();
    test_sleep_exact_minimum();
    test_sleep_bad_number();
    test_wake_dumps();
    test_run_missing_file();
    test_run_wrong_extension();
    test_unknown_command();

    if (failures != 0) {
        std::cerr << failures << " expectation(s) failed. You are still asleep.\n";
        return 1;
    }
    std::cout << "all dreams passed\n";
    return 0;
}

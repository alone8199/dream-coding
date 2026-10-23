// dc/runner.hpp
// dc 命令行运行器的公共接口。
//
// 编译于潜意识，运行于不存在。

#pragma once

#include <cstddef>
#include <ostream>
#include <string>
#include <vector>

namespace dc {

inline constexpr const char* kVersion = "0.2.0";
inline constexpr const char* kDumpMessage = "Segmentation fault (dream dumped)";
inline constexpr double kDefaultSleepHours = 8.0;
inline constexpr double kMinSleepHours = 7.0;

enum ExitCode : int {
    kExitOk = 0,
    kExitSleepTooShort = 1,
    kExitUsage = 2,
    kExitFileError = 3,
};

// 对一份 .dc 源文件做"潜意识编译"时的粗略检查结果。
struct CompileReport {
    std::size_t lines = 0;
    std::size_t comments = 0;
    std::size_t depth_errors = 0;  // 多余的 '}'
    long final_depth = 0;          // 结束时仍未闭合的 '{'

    bool balanced() const { return depth_errors == 0 && final_depth == 0; }
};

// 扫描源码，统计行数与注释，并检查花括号是否配对。
// 字符串与注释中的括号会被忽略。
CompileReport analyze(const std::string& source);

// 路径是否以 ".dc" 结尾（大小写不敏感）。
bool has_dc_extension(const std::string& path);

// 执行一条命令，返回退出码。
// args 不包含程序名本身，例如 {"run", "main.dc"}。
int run_command(const std::vector<std::string>& args, std::ostream& out, std::ostream& err);

}  // namespace dc

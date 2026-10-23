// dc/runner.cpp
// dc 命令行运行器的实现。

#include "dc/runner.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <exception>
#include <fstream>
#include <sstream>

namespace dc {

namespace {

constexpr const char* kUsage =
    "usage: dc <command> [args]\n"
    "\n"
    "commands:\n"
    "  sleep [hours]   enter the REM cycle (default 8h, minimum 7h)\n"
    "  run <file.dc>   compile in subconscious, run in nonexistent\n"
    "  wake            wake up and find the segfault\n"
    "  version         print version\n"
    "  help            show this message\n";

bool parse_hours(const std::string& text, double& out) {
    try {
        std::size_t used = 0;
        out = std::stod(text, &used);
        return used == text.size() && std::isfinite(out) && out >= 0.0;
    } catch (const std::exception&) {
        return false;
    }
}

int cmd_sleep(const std::vector<std::string>& args, std::ostream& out, std::ostream& err) {
    double hours = kDefaultSleepHours;
    if (args.size() > 1 && !parse_hours(args[1], hours)) {
        err << "dc: sleep: hours must be a non-negative number, got '" << args[1] << "'\n";
        return kExitUsage;
    }

    if (hours < kMinSleepHours) {
        err << "TimeoutError: sleep too short (" << hours << "h < " << kMinSleepHours << "h)\n";
        return kExitSleepTooShort;
    }

    out << "Entering REM cycle...\n";
    out << "zzz... (" << hours << "h)\n";
    out << "Dreams recorded: 0 (you will forget them)\n";
    return kExitOk;
}

int cmd_run(const std::vector<std::string>& args, std::ostream& out, std::ostream& err) {
    if (args.size() < 2) {
        err << "dc: run: missing source file\n";
        return kExitUsage;
    }

    const std::string& path = args[1];
    if (!has_dc_extension(path)) {
        err << "dc: run: '" << path << "' is not a .dc file (no dream without the right suffix)\n";
        return kExitUsage;
    }

    std::ifstream in(path, std::ios::binary);
    if (!in) {
        err << "FileNotFound: no dream at '" << path << "'\n";
        return kExitFileError;
    }

    std::ostringstream buffer;
    buffer << in.rdbuf();
    const CompileReport report = analyze(buffer.str());

    if (report.lines == 0) {
        out << "(empty dream)\n";
    }

    out << "Compiling in subconscious... ";
    if (report.balanced()) {
        out << "done\n";
    } else {
        const long problems = static_cast<long>(report.depth_errors) + report.final_depth;
        out << "done (" << problems << " missing braces filled by intuition)\n";
    }

    out << "Running in nonexistent... done\n";
    out << "Waking up... done\n\n";
    out << kDumpMessage << "\n";
    return kExitOk;
}

int cmd_wake(std::ostream& out) {
    out << "Waking up...\n";
    out << kDumpMessage << "\n";
    return kExitOk;
}

}  // namespace

CompileReport analyze(const std::string& source) {
    enum class Mode { Code, LineComment, String };

    CompileReport report;
    if (!source.empty()) {
        report.lines = static_cast<std::size_t>(std::count(source.begin(), source.end(), '\n'));
        if (source.back() != '\n') {
            ++report.lines;
        }
    }

    Mode mode = Mode::Code;
    long depth = 0;

    for (std::size_t i = 0; i < source.size(); ++i) {
        const char c = source[i];
        switch (mode) {
            case Mode::Code:
                if (c == '/' && i + 1 < source.size() && source[i + 1] == '/') {
                    mode = Mode::LineComment;
                    ++report.comments;
                    ++i;
                } else if (c == '"') {
                    mode = Mode::String;
                } else if (c == '{') {
                    ++depth;
                } else if (c == '}') {
                    if (depth == 0) {
                        ++report.depth_errors;
                    } else {
                        --depth;
                    }
                }
                break;

            case Mode::LineComment:
                if (c == '\n') {
                    mode = Mode::Code;
                }
                break;

            case Mode::String:
                if (c == '\\') {
                    ++i;  // 跳过被转义的字符
                } else if (c == '"' || c == '\n') {
                    mode = Mode::Code;  // 未闭合的字符串在行尾结束
                }
                break;
        }
    }

    report.final_depth = depth;
    return report;
}

bool has_dc_extension(const std::string& path) {
    static const std::string kExt = ".dc";
    if (path.size() <= kExt.size()) {
        return false;
    }
    std::string tail = path.substr(path.size() - kExt.size());
    for (char& ch : tail) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    return tail == kExt;
}

int run_command(const std::vector<std::string>& args, std::ostream& out, std::ostream& err) {
    if (args.empty()) {
        err << kUsage;
        return kExitUsage;
    }

    const std::string& command = args[0];

    if (command == "help" || command == "--help" || command == "-h") {
        out << kUsage;
        return kExitOk;
    }
    if (command == "version" || command == "--version") {
        out << "dc " << kVersion << "\n";
        return kExitOk;
    }
    if (command == "sleep") {
        return cmd_sleep(args, out, err);
    }
    if (command == "run") {
        return cmd_run(args, out, err);
    }
    if (command == "wake") {
        return cmd_wake(out);
    }

    err << "dc: unknown command '" << command << "'\n" << kUsage;
    return kExitUsage;
}

}  // namespace dc

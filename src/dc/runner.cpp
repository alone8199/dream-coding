// dc/runner.cpp
// dc 命令行运行器的实现。

#include "dc/runner.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <exception>
#include <fstream>
#include <sstream>

#include "dc/error.hpp"
#include "dc/interpreter.hpp"
#include "dc/parser.hpp"

namespace dc {
namespace {

constexpr const char* kUsage =
    "usage: dc <command> [args]\n"
    "\n"
    "commands:\n"
    "  run <file.dc> [args...]  run a script (extra args go to argv)\n"
    "  check <file.dc>          parse a script without running it\n"
    "  eval <code>              run a line of code\n"
    "  sleep [hours]            enter the REM cycle (default 8h, minimum 7h)\n"
    "  wake                     wake up and find the segfault\n"
    "  version                  print version\n"
    "  help                     show this message\n";

bool read_file(const std::string& path, std::string& text) {
  std::ifstream in(path, std::ios::binary);
  if (!in) return false;
  std::ostringstream buf;
  buf << in.rdbuf();
  text = buf.str();
  return true;
}

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

  std::string text;
  if (!read_file(path, text)) {
    err << "FileNotFound: no dream at '" << path << "'\n";
    return kExitFileError;
  }

  Interpreter interp(out);

  // 脚本可以通过全局变量 argv 读取命令行参数（不含 dc 和 run）
  std::vector<Value> script_args;
  for (std::size_t i = 2; i < args.size(); ++i) {
    script_args.push_back(Value::str(args[i]));
  }
  interp.define_global("argv", Value::list(std::move(script_args)));

  try {
    interp.run_file(path);
  } catch (const DcError& e) {
    err << path << ":" << e.line() << ": " << e.what() << "\n";
    err << kDumpMessage << "\n";
    return kExitProgram;
  }
  return kExitOk;
}

int cmd_eval(const std::vector<std::string>& args, std::ostream& out, std::ostream& err) {
  if (args.size() < 2) {
    err << "dc: eval: missing code\n";
    return kExitUsage;
  }
  Interpreter interp(out);
  try {
    interp.run(args[1]);
  } catch (const DcError& e) {
    err << "line " << e.line() << ": " << e.what() << "\n";
    return kExitProgram;
  }
  return kExitOk;
}

int cmd_check(const std::vector<std::string>& args, std::ostream& out, std::ostream& err) {
  if (args.size() < 2) {
    err << "dc: check: missing source file\n";
    return kExitUsage;
  }

  const std::string& path = args[1];
  std::string text;
  if (!read_file(path, text)) {
    err << "FileNotFound: no dream at '" << path << "'\n";
    return kExitFileError;
  }

  try {
    parse_program(text);
  } catch (const DcError& e) {
    err << path << ":" << e.line() << ": " << e.what() << "\n";
    return kExitProgram;
  }
  out << path << ": ok\n";
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
        if (c == '\n') mode = Mode::Code;
        break;

      case Mode::String:
        if (c == '\\') {
          ++i;  // 跳过被转义的字符
        } else if (c == '"' || c == '\n') {
          mode = Mode::Code;
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
  if (command == "run") return cmd_run(args, out, err);
  if (command == "eval") return cmd_eval(args, out, err);
  if (command == "check") return cmd_check(args, out, err);
  if (command == "sleep") return cmd_sleep(args, out, err);
  if (command == "wake") {
    out << "Waking up...\n" << kDumpMessage << "\n";
    return kExitOk;
  }

  err << "dc: unknown command '" << command << "'\n" << kUsage;
  return kExitUsage;
}

}  // namespace dc

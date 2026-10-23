// tests/interp_test.cpp
// 解释器的端到端测试：写 dc 代码，检查输出或错误。

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "dc/error.hpp"
#include "dc/interpreter.hpp"

namespace {

int failures = 0;
int checks = 0;

void expect_eq(const std::string& actual, const std::string& expected, int line) {
  ++checks;
  if (actual != expected) {
    ++failures;
    std::cerr << "line " << line << ": expected [" << expected << "] got [" << actual << "]\n";
  }
}

void expect_true(bool cond, const char* what, int line) {
  ++checks;
  if (!cond) {
    ++failures;
    std::cerr << "line " << line << ": expected " << what << "\n";
  }
}

// 运行代码，返回标准输出。
std::string run(const std::string& src) {
  std::ostringstream out;
  dc::Interpreter interp(out);
  interp.run(src);
  return out.str();
}

// 运行代码，期望出错，返回 "消息 @行号"。
std::string run_error(const std::string& src) {
  std::ostringstream out;
  dc::Interpreter interp(out);
  try {
    interp.run(src);
  } catch (const dc::DcError& e) {
    return std::string(e.what()) + " @" + std::to_string(e.line());
  }
  return "(no error)";
}

#define EQ(actual, expected) expect_eq((actual), (expected), __LINE__)
#define TRUE_(cond) expect_true((cond), #cond, __LINE__)

void test_arithmetic() {
  EQ(run("print(1 + 2 * 3);"), "7\n");
  EQ(run("print((1 + 2) * 3);"), "9\n");
  EQ(run("print(7 / 2);"), "3.5\n");
  EQ(run("print(7 % 3, -4 + 1);"), "1 -3\n");
  EQ(run("print(2 * 3 == 6, 1 != 1);"), "true false\n");
}

void test_strings() {
  EQ(run("print(\"ab\" + \"cd\", len(\"hello\"));"), "abcd 5\n");
  EQ(run("print(\"ha\" * 3);"), "hahaha\n");
  EQ(run("print(\"a,b\".split(\",\")[1].upper());"), "B\n");
  EQ(run("print(\"  x \".strip() + \"!\");"), "x!\n");
  EQ(run("print(\"tab\\there\");"), "tab\there\n");
  EQ(run("print(\"中文变量\");"), "中文变量\n");
}

void test_variables_and_scope() {
  EQ(run("let x = 1; x = x + 1; print(x);"), "2\n");
  EQ(run("let x = 1; { let x = 5; print(x); } print(x);"), "5\n1\n");
  EQ(run_error("y = 3;"), "NameError: name 'y' is not defined (use let first) @1");
}

void test_control_flow() {
  EQ(run("if (1 > 2) { print(\"a\"); } else { print(\"b\"); }"), "b\n");
  EQ(run("if false { print(1); } else if true { print(2); } else { print(3); }"), "2\n");
  EQ(run("let i = 0; while (true) { i = i + 1; if (i == 2) { continue; } if (i > 4) { break; } "
         "print(i); }"),
     "1\n3\n4\n");
  EQ(run("for x in [1, 2] { print(x); }"), "1\n2\n");
  EQ(run("for i in range(3) { print(i); }"), "0\n1\n2\n");
  EQ(run("for i in range(10, 0, -4) { print(i); }"), "10\n6\n2\n");
}

void test_functions() {
  EQ(run("fn add(a, b) { return a + b; } print(add(2, 3));"), "5\n");
  EQ(run("fn fib(n) { if (n < 2) { return n; } return fib(n - 1) + fib(n - 2); } print(fib(15));"),
     "610\n");
  EQ(run("fn make() { let c = 0; return fn() { c = c + 1; return c; }; } "
         "let f = make(); f(); print(f());"),
     "2\n");
  EQ(run("fn apply(g, x) { return g(x); } print(apply(fn(v) { return v * 10; }, 4));"), "40\n");
  EQ(run("fn nothing() { } print(nothing());"), "null\n");
  EQ(run_error("fn f(a) { return a; } f(1, 2);"),
     "TypeError: f() takes 1 argument(s) but 2 were given @1");
}

void test_lists() {
  EQ(run("let xs = []; xs.push(1); xs.push(2); print(xs.pop(), len(xs));"), "2 1\n");
  EQ(run("print([1, 2, 3][-1], [1, 2, 3][0]);"), "3 1\n");
  EQ(run("print([1, 2] + [3]);"), "[1, 2, 3]\n");
  EQ(run("let xs = [1, 2]; xs[0] = 9; print(xs);"), "[9, 2]\n");
  EQ(run("print(2 in [1, 2], 5 in [1, 2]);"), "true false\n");
  EQ(run("print([\"a\", \"b\"].join(\"-\"));"), "a-b\n");
  EQ(run_error("print([1][5]);"), "IndexError: index 5 out of range @1");
}

void test_dicts() {
  EQ(run("let d = {\"a\": 1, b: 2}; d[\"c\"] = 3; print(d.a, d[\"c\"], len(d));"), "1 3 3\n");
  EQ(run("let d = {x: 1}; d.y = 2; print(d.x + d.y);"), "3\n");
  EQ(run("let d = {x: 1}; print(d.get(\"nope\", 7), d.has(\"x\"));"), "7 true\n");
  EQ(run("print({k: [1]});"), "{\"k\": [1]}\n");
  EQ(run("for k in {a: 1, b: 2} { print(k); }"), "a\nb\n");
  EQ(run_error("let d = {}; print(d[\"zz\"]);"), "KeyError: \"zz\" @1");
}

void test_logic_short_circuit() {
  EQ(run("print(false and boom());"), "false\n");
  EQ(run("print(true or boom());"), "true\n");
  EQ(run("print(not true, !false);"), "false true\n");
  EQ(run("print(0 || \"fallback\");"), "fallback\n");
}

void test_errors_have_lines() {
  EQ(run_error("let a = 1;\nlet b = 1 / 0;"), "ZeroDivisionError: division by zero @2");
  EQ(run_error("print(1 < \"a\");"),
     "TypeError: unsupported operand type(s) for '<': 'number' and 'string' @1");
  EQ(run_error("print(zzz);"), "NameError: name 'zzz' is not defined @1");
  EQ(run_error("assert(1 == 2, \"math is broken\");"), "AssertionError: math is broken @1");
  EQ(run_error("raise(\"boom\");"), "Error: boom @1");
}

void test_syntax_errors() {
  const std::string err = run_error("let = 3;");
  TRUE_(err.find("SyntaxError") == 0);
  TRUE_(err.find("@1") != std::string::npos);

  TRUE_(run_error("fn f() { ").find("SyntaxError") == 0);
  TRUE_(run_error("1 = 2;").find("SyntaxError") == 0);
  TRUE_(run_error("let s = \"unterminated;").find("SyntaxError") == 0);
}

void test_recursion_limit() {
  const std::string err = run_error("fn f(n) { return f(n + 1); } f(0);");
  TRUE_(err.find("RecursionError") != std::string::npos);
}

void test_builtins_and_types() {
  EQ(run("print(type(1), type(\"x\"), type([]), type({}), type(null), type(print));"),
     "number string list dict null function\n");
  EQ(run("print(num(\"2.5\") + 1, int(3.9), str(12) + \"!\");"), "3.5 3 12!\n");
  EQ(run("print(null == null, [1] == [1]);"), "true false\n");
}

void test_math_module() {
  EQ(run("import math; print(math.sqrt(16), math.max(1, 9, 3), math.min([4, 2]));"), "4 9 2\n");
  EQ(run("import math; print(math.floor(2.7), math.abs(-3));"), "2 3\n");
}

void test_dream_module() {
  EQ(run("import dream; dream.wake();"), "Segmentation fault (dream dumped)\n");
  EQ(run("import dream; print(dream.sleep(8));"), "Entering REM cycle...\nzzz\n");
  EQ(run_error("import dream; dream.sleep(4);"), "TimeoutError: sleep too short (4h < 7h) @1");
}

void test_native_extension() {
  std::ostringstream out;
  dc::Interpreter interp(out);

  interp.define_function("double_it", [](dc::Interpreter&, std::vector<dc::Value>& a, int) {
    return dc::Value::num(std::get<double>(a[0].data) * 2);
  });

  interp.register_module("greet", [](dc::Interpreter&) {
    dc::Value m = dc::Value::dict();
    std::get<dc::DictPtr>(m.data)->items["hi"] = dc::make_native(
        "hi", [](dc::Interpreter&, std::vector<dc::Value>& a, int) {
          return dc::Value::str("hi, " + dc::to_display(a[0]));
        });
    return m;
  });

  interp.run("import greet; print(greet.hi(\"dc\")); print(double_it(21));");
  EQ(out.str(), "hi, dc\n42\n");
}

void test_file_modules() {
  namespace fs = std::filesystem;
  const fs::path dir = fs::temp_directory_path() / "dc_interp_test_modules";
  fs::create_directories(dir / "lib");

  {
    std::ofstream lib(dir / "lib" / "util.dc");
    lib << "fn twice(x) { return x * 2; }\n"
        << "let _hidden = 1;\n"
        << "let visible = \"yes\";\n";
  }
  {
    std::ofstream main(dir / "main.dc");
    main << "import \"lib/util.dc\";\n"
         << "print(util.twice(4), util.visible);\n";
  }

  std::ostringstream out;
  dc::Interpreter interp(out);
  interp.run_file((dir / "main.dc").string());
  EQ(out.str(), "8 yes\n");

  std::ostringstream out2;
  dc::Interpreter interp2(out2);
  std::string err;
  try {
    interp2.run("import \"" + (dir / "lib" / "util.dc").generic_string() + "\"; print(util._hidden);");
  } catch (const dc::DcError& e) {
    err = e.what();
  }
  TRUE_(err.find("AttributeError") == 0);

  fs::remove_all(dir);
}

void test_import_missing() {
  TRUE_(run_error("import nowhere_to_be_found;").find("ImportError") == 0);
}

}  // namespace

int main() {
  test_arithmetic();
  test_strings();
  test_variables_and_scope();
  test_control_flow();
  test_functions();
  test_lists();
  test_dicts();
  test_logic_short_circuit();
  test_errors_have_lines();
  test_syntax_errors();
  test_recursion_limit();
  test_builtins_and_types();
  test_math_module();
  test_dream_module();
  test_native_extension();
  test_file_modules();
  test_import_missing();

  if (failures != 0) {
    std::cerr << failures << " of " << checks << " checks failed. You are still asleep.\n";
    return 1;
  }
  std::cout << "all " << checks << " dreams passed\n";
  return 0;
}

// dc/stdlib.cpp
// 标准库模块：用 import 加载。
//   import math;   math.sqrt(16)
//   import dream;  dream.sleep(8); dream.wake()
//
// 想增加自己的模块，用 Interpreter::register_module，见 docs/extending.md。

#include <algorithm>
#include <cmath>
#include <map>
#include <string>
#include <vector>

#include "dc/error.hpp"
#include "dc/interpreter.hpp"

namespace dc {
namespace {

constexpr double kPi = 3.14159265358979323846;
constexpr double kE = 2.71828182845904523536;

void put(Value& module, const std::string& name, Value v) {
  std::get<DictPtr>(module.data)->items[name] = std::move(v);
}

double num_arg(const std::vector<Value>& a, size_t i, const char* fname, int line) {
  if (i >= a.size() || !a[i].is_num()) {
    throw DcError(std::string("TypeError: ") + fname + "() expects a number", line);
  }
  return std::get<double>(a[i].data);
}

// min / max 既可以 min(1, 2, 3)，也可以 min([1, 2, 3])
std::vector<double> numbers_from(const std::vector<Value>& a, const char* fname, int line) {
  std::vector<Value> source = a;
  if (a.size() == 1 && a[0].is_list()) {
    source = std::get<ListPtr>(a[0].data)->items;
  }
  std::vector<double> out;
  for (const Value& v : source) {
    if (!v.is_num()) throw DcError(std::string("TypeError: ") + fname + "() expects numbers", line);
    out.push_back(std::get<double>(v.data));
  }
  if (out.empty()) throw DcError(std::string("ValueError: ") + fname + "() of empty sequence", line);
  return out;
}

Value math_module() {
  Value m = Value::dict();
  put(m, "pi", Value::num(kPi));
  put(m, "e", Value::num(kE));

  put(m, "sqrt", make_native("sqrt", [](Interpreter&, std::vector<Value>& a, int line) {
    const double x = num_arg(a, 0, "sqrt", line);
    if (x < 0) throw DcError("ValueError: math domain error", line);
    return Value::num(std::sqrt(x));
  }));
  put(m, "floor", make_native("floor", [](Interpreter&, std::vector<Value>& a, int line) {
    return Value::num(std::floor(num_arg(a, 0, "floor", line)));
  }));
  put(m, "ceil", make_native("ceil", [](Interpreter&, std::vector<Value>& a, int line) {
    return Value::num(std::ceil(num_arg(a, 0, "ceil", line)));
  }));
  put(m, "abs", make_native("abs", [](Interpreter&, std::vector<Value>& a, int line) {
    return Value::num(std::fabs(num_arg(a, 0, "abs", line)));
  }));
  put(m, "pow", make_native("pow", [](Interpreter&, std::vector<Value>& a, int line) {
    return Value::num(std::pow(num_arg(a, 0, "pow", line), num_arg(a, 1, "pow", line)));
  }));
  put(m, "min", make_native("min", [](Interpreter&, std::vector<Value>& a, int line) {
    const auto xs = numbers_from(a, "min", line);
    return Value::num(*std::min_element(xs.begin(), xs.end()));
  }));
  put(m, "max", make_native("max", [](Interpreter&, std::vector<Value>& a, int line) {
    const auto xs = numbers_from(a, "max", line);
    return Value::num(*std::max_element(xs.begin(), xs.end()));
  }));
  return m;
}

Value dream_module() {
  Value m = Value::dict();
  put(m, "dump", Value::str(kDumpMessage));

  // dream.sleep(hours)：少于七小时会超时，和 dc sleep 的规则一致
  put(m, "sleep", make_native("sleep", [](Interpreter& in, std::vector<Value>& a, int line) {
    const double hours = a.empty() ? 8.0 : num_arg(a, 0, "sleep", line);
    if (hours < 7.0) {
      throw DcError("TimeoutError: sleep too short (" + format_number(hours) + "h < 7h)", line);
    }
    in.out() << "Entering REM cycle...\n";
    return Value::str("zzz");
  }));

  // dream.wake()：醒来，终端里只剩一行字
  put(m, "wake", make_native("wake", [](Interpreter& in, std::vector<Value>&, int) {
    in.out() << kDumpMessage << '\n';
    return Value::nil();
  }));
  return m;
}

}  // namespace

void install_stdlib(Interpreter& in) {
  in.register_module("math", [](Interpreter&) { return math_module(); });
  in.register_module("dream", [](Interpreter&) { return dream_module(); });
}

}  // namespace dc

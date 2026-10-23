// dc/builtins.cpp
// 全局内置函数，以及列表、字符串、字典的方法。

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <optional>
#include <string>
#include <vector>

#include "dc/error.hpp"
#include "dc/interpreter.hpp"

namespace dc {
namespace {

void need_args(const std::vector<Value>& args, size_t lo, size_t hi, const std::string& name,
               int line) {
  if (args.size() < lo || args.size() > hi) {
    throw DcError("TypeError: " + name + "() takes " + std::to_string(lo) +
                      (lo == hi ? "" : ".." + std::to_string(hi)) + " argument(s) but " +
                      std::to_string(args.size()) + " were given",
                  line);
  }
}

double number_arg(const Value& v, const std::string& name, int line) {
  if (!v.is_num()) {
    throw DcError("TypeError: " + name + "() expects a number, got '" + v.type_name() + "'", line);
  }
  return std::get<double>(v.data);
}

const std::string& string_arg(const Value& v, const std::string& name, int line) {
  if (!v.is_str()) {
    throw DcError("TypeError: " + name + "() expects a string, got '" + v.type_name() + "'", line);
  }
  return std::get<std::string>(v.data);
}

Value num_from_string(const std::string& s, int line) {
  char* end = nullptr;
  const double d = std::strtod(s.c_str(), &end);
  if (s.empty() || end != s.c_str() + s.size()) {
    throw DcError("ValueError: could not convert string to number: " + to_repr(Value::str(s)),
                  line);
  }
  return Value::num(d);
}

}  // namespace

void install_builtins(Interpreter& in) {
  in.define_function("print", [](Interpreter& it, std::vector<Value>& args, int) {
    for (size_t i = 0; i < args.size(); ++i) {
      if (i > 0) it.out() << ' ';
      it.out() << to_display(args[i]);
    }
    it.out() << '\n';
    return Value::nil();
  });

  in.define_function("len", [](Interpreter&, std::vector<Value>& a, int line) {
    need_args(a, 1, 1, "len", line);
    const Value& v = a[0];
    if (v.is_str()) return Value::num(static_cast<double>(std::get<std::string>(v.data).size()));
    if (v.is_list()) return Value::num(static_cast<double>(std::get<ListPtr>(v.data)->items.size()));
    if (v.is_dict()) return Value::num(static_cast<double>(std::get<DictPtr>(v.data)->items.size()));
    throw DcError(std::string("TypeError: object of type '") + v.type_name() + "' has no len()",
                  line);
  });

  in.define_function("str", [](Interpreter&, std::vector<Value>& a, int line) {
    need_args(a, 1, 1, "str", line);
    return Value::str(to_display(a[0]));
  });

  in.define_function("num", [](Interpreter&, std::vector<Value>& a, int line) {
    need_args(a, 1, 1, "num", line);
    const Value& v = a[0];
    if (v.is_num()) return v;
    if (v.is_bool()) return Value::num(std::get<bool>(v.data) ? 1.0 : 0.0);
    if (v.is_str()) return num_from_string(std::get<std::string>(v.data), line);
    throw DcError(std::string("TypeError: cannot convert '") + v.type_name() + "' to number", line);
  });

  in.define_function("int", [](Interpreter&, std::vector<Value>& a, int line) {
    need_args(a, 1, 1, "int", line);
    const Value& v = a[0];
    if (v.is_str()) return Value::num(std::trunc(std::get<double>(num_from_string(std::get<std::string>(v.data), line).data)));
    return Value::num(std::trunc(number_arg(v, "int", line)));
  });

  in.define_function("type", [](Interpreter&, std::vector<Value>& a, int line) {
    need_args(a, 1, 1, "type", line);
    return Value::str(a[0].type_name());
  });

  in.define_function("range", [](Interpreter&, std::vector<Value>& a, int line) {
    need_args(a, 1, 3, "range", line);
    double start = 0.0;
    double stop = 0.0;
    double step = 1.0;
    if (a.size() == 1) {
      stop = number_arg(a[0], "range", line);
    } else {
      start = number_arg(a[0], "range", line);
      stop = number_arg(a[1], "range", line);
      if (a.size() == 3) step = number_arg(a[2], "range", line);
    }
    if (step == 0.0) throw DcError("ValueError: range() step must not be zero", line);

    std::vector<Value> items;
    for (double x = start; step > 0 ? x < stop : x > stop; x += step) {
      items.push_back(Value::num(x));
    }
    return Value::list(std::move(items));
  });

  in.define_function("list", [](Interpreter&, std::vector<Value>& a, int line) {
    need_args(a, 1, 1, "list", line);
    return Value::list(iterate(a[0], line));
  });

  in.define_function("assert", [](Interpreter&, std::vector<Value>& a, int line) {
    need_args(a, 1, 2, "assert", line);
    if (!a[0].truthy()) {
      const std::string msg = a.size() > 1 ? to_display(a[1]) : "assertion failed";
      throw DcError("AssertionError: " + msg, line);
    }
    return Value::nil();
  });

  in.define_function("raise", [](Interpreter&, std::vector<Value>& a, int line) -> Value {
    need_args(a, 1, 1, "raise", line);
    throw DcError("Error: " + to_display(a[0]), line);
  });

  in.define_function("time", [](Interpreter&, std::vector<Value>&, int) {
    using namespace std::chrono;
    const auto now = system_clock::now().time_since_epoch();
    return Value::num(duration<double>(now).count());
  });
}

std::optional<Value> call_method(Interpreter& in, const Value& obj, const std::string& name,
                                 std::vector<Value>& args, int line) {
  (void)in;

  if (obj.is_list()) {
    auto& items = std::get<ListPtr>(obj.data)->items;

    if (name == "push") {
      need_args(args, 1, 1, "push", line);
      items.push_back(args[0]);
      return Value::nil();
    }
    if (name == "pop") {
      need_args(args, 0, 0, "pop", line);
      if (items.empty()) throw DcError("IndexError: pop from empty list", line);
      Value v = items.back();
      items.pop_back();
      return v;
    }
    if (name == "reverse") {
      need_args(args, 0, 0, "reverse", line);
      std::reverse(items.begin(), items.end());
      return Value::nil();
    }
    if (name == "join") {
      need_args(args, 0, 1, "join", line);
      const std::string sep = args.empty() ? "" : string_arg(args[0], "join", line);
      std::string out;
      for (size_t i = 0; i < items.size(); ++i) {
        if (i > 0) out += sep;
        out += to_display(items[i]);
      }
      return Value::str(std::move(out));
    }
    return std::nullopt;
  }

  if (obj.is_str()) {
    const std::string& s = std::get<std::string>(obj.data);

    if (name == "upper" || name == "lower") {
      need_args(args, 0, 0, name, line);
      std::string out = s;
      for (char& c : out) {
        c = static_cast<char>(name == "upper" ? std::toupper(static_cast<unsigned char>(c))
                                              : std::tolower(static_cast<unsigned char>(c)));
      }
      return Value::str(std::move(out));
    }
    if (name == "strip") {
      need_args(args, 0, 0, "strip", line);
      const size_t b = s.find_first_not_of(" \t\r\n");
      if (b == std::string::npos) return Value::str("");
      const size_t e = s.find_last_not_of(" \t\r\n");
      return Value::str(s.substr(b, e - b + 1));
    }
    if (name == "split") {
      need_args(args, 0, 1, "split", line);
      std::vector<Value> parts;
      if (args.empty()) {
        size_t i = 0;
        while (i < s.size()) {
          while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
          const size_t start = i;
          while (i < s.size() && !std::isspace(static_cast<unsigned char>(s[i]))) ++i;
          if (i > start) parts.push_back(Value::str(s.substr(start, i - start)));
        }
      } else {
        const std::string sep = string_arg(args[0], "split", line);
        if (sep.empty()) throw DcError("ValueError: empty separator", line);
        size_t start = 0;
        while (true) {
          const size_t hit = s.find(sep, start);
          if (hit == std::string::npos) {
            parts.push_back(Value::str(s.substr(start)));
            break;
          }
          parts.push_back(Value::str(s.substr(start, hit - start)));
          start = hit + sep.size();
        }
      }
      return Value::list(std::move(parts));
    }
    if (name == "replace") {
      need_args(args, 2, 2, "replace", line);
      std::string from = string_arg(args[0], "replace", line);
      const std::string to = string_arg(args[1], "replace", line);
      if (from.empty()) return Value::str(s);
      std::string out;
      size_t start = 0;
      while (true) {
        const size_t hit = s.find(from, start);
        if (hit == std::string::npos) {
          out += s.substr(start);
          break;
        }
        out += s.substr(start, hit - start) + to;
        start = hit + from.size();
      }
      return Value::str(std::move(out));
    }
    if (name == "startswith" || name == "endswith") {
      need_args(args, 1, 1, name, line);
      const std::string& p = string_arg(args[0], name, line);
      if (p.size() > s.size()) return Value::boolean(false);
      if (name == "startswith") return Value::boolean(s.compare(0, p.size(), p) == 0);
      return Value::boolean(s.compare(s.size() - p.size(), p.size(), p) == 0);
    }
    return std::nullopt;
  }

  if (obj.is_dict()) {
    auto& items = std::get<DictPtr>(obj.data)->items;

    if (name == "keys") {
      need_args(args, 0, 0, "keys", line);
      std::vector<Value> out;
      for (const auto& [k, v] : items) out.push_back(Value::str(k));
      return Value::list(std::move(out));
    }
    if (name == "values") {
      need_args(args, 0, 0, "values", line);
      std::vector<Value> out;
      for (const auto& [k, v] : items) out.push_back(v);
      return Value::list(std::move(out));
    }
    if (name == "has") {
      need_args(args, 1, 1, "has", line);
      return Value::boolean(items.count(dict_key(args[0], line)) > 0);
    }
    if (name == "get") {
      need_args(args, 1, 2, "get", line);
      auto it = items.find(dict_key(args[0], line));
      if (it != items.end()) return it->second;
      return args.size() > 1 ? args[1] : Value::nil();
    }
    return std::nullopt;
  }

  return std::nullopt;
}

}  // namespace dc

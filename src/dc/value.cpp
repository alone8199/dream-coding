// dc/value.cpp

#include "dc/value.hpp"

#include <cmath>
#include <cstdio>
#include <sstream>

namespace dc {

bool Value::truthy() const {
  if (is_nil()) return false;
  if (is_bool()) return std::get<bool>(data);
  if (is_num()) return std::get<double>(data) != 0.0;
  if (is_str()) return !std::get<std::string>(data).empty();
  if (is_list()) return !std::get<ListPtr>(data)->items.empty();
  if (is_dict()) return !std::get<DictPtr>(data)->items.empty();
  return true;
}

const char* Value::type_name() const {
  if (is_nil()) return "null";
  if (is_bool()) return "bool";
  if (is_num()) return "number";
  if (is_str()) return "string";
  if (is_list()) return "list";
  if (is_dict()) return "dict";
  return "function";
}

std::string format_number(double d) {
  if (std::isfinite(d) && std::floor(d) == d && std::fabs(d) < 1e15) {
    char buf[32];
    std::snprintf(buf, sizeof buf, "%.0f", d);
    return buf;
  }
  std::ostringstream os;
  os.precision(15);
  os << d;
  return os.str();
}

namespace {

std::string quote(const std::string& s) {
  std::string out = "\"";
  for (char c : s) {
    switch (c) {
      case '"': out += "\\\""; break;
      case '\\': out += "\\\\"; break;
      case '\n': out += "\\n"; break;
      case '\t': out += "\\t"; break;
      default: out += c; break;
    }
  }
  out += '"';
  return out;
}

}  // namespace

std::string to_repr(const Value& v) {
  if (v.is_str()) return quote(std::get<std::string>(v.data));

  if (v.is_list()) {
    std::string out = "[";
    bool first = true;
    for (const Value& item : std::get<ListPtr>(v.data)->items) {
      if (!first) out += ", ";
      first = false;
      out += to_repr(item);
    }
    return out + "]";
  }

  if (v.is_dict()) {
    std::string out = "{";
    bool first = true;
    for (const auto& [key, value] : std::get<DictPtr>(v.data)->items) {
      if (!first) out += ", ";
      first = false;
      out += quote(key) + ": " + to_repr(value);
    }
    return out + "}";
  }

  if (v.is_func()) return "<fn " + std::get<CallPtr>(v.data)->name() + ">";

  return to_display(v);
}

std::string to_display(const Value& v) {
  if (v.is_nil()) return "null";
  if (v.is_bool()) return std::get<bool>(v.data) ? "true" : "false";
  if (v.is_num()) return format_number(std::get<double>(v.data));
  if (v.is_str()) return std::get<std::string>(v.data);
  return to_repr(v);
}

bool values_equal(const Value& a, const Value& b) {
  // variant 比较：类型不同为假；列表、字典、函数比较的是指针（同一对象才相等）。
  return a.data == b.data;
}

}  // namespace dc

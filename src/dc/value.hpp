// dc/value.hpp
// 运行时的值。数字统一用 double，字符串、列表、字典、函数都是引用语义（和 Python 一样）。

#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace dc {

class Interpreter;
struct Value;
struct ListObj;
struct DictObj;
struct Callable;

using ListPtr = std::shared_ptr<ListObj>;
using DictPtr = std::shared_ptr<DictObj>;
using CallPtr = std::shared_ptr<Callable>;

// 醒来后终端里唯一的一行字。
inline constexpr const char* kDumpMessage = "Segmentation fault (dream dumped)";

struct Value {
  std::variant<std::monostate, bool, double, std::string, ListPtr, DictPtr, CallPtr> data;

  static Value nil() { return Value{}; }
  static Value boolean(bool b) {
    Value v;
    v.data = b;
    return v;
  }
  static Value num(double d) {
    Value v;
    v.data = d;
    return v;
  }
  static Value str(std::string s) {
    Value v;
    v.data = std::move(s);
    return v;
  }
  static Value list(std::vector<Value> items = {});
  static Value dict();
  static Value func(CallPtr f) {
    Value v;
    v.data = std::move(f);
    return v;
  }

  bool is_nil() const { return std::holds_alternative<std::monostate>(data); }
  bool is_bool() const { return std::holds_alternative<bool>(data); }
  bool is_num() const { return std::holds_alternative<double>(data); }
  bool is_str() const { return std::holds_alternative<std::string>(data); }
  bool is_list() const { return std::holds_alternative<ListPtr>(data); }
  bool is_dict() const { return std::holds_alternative<DictPtr>(data); }
  bool is_func() const { return std::holds_alternative<CallPtr>(data); }

  bool truthy() const;
  const char* type_name() const;
};

// 可调用对象：用户函数和 C++ 原生函数都实现这个接口。
struct Callable {
  virtual ~Callable() = default;
  virtual std::string name() const = 0;
  virtual Value call(Interpreter& in, std::vector<Value>& args, int line) = 0;
};

// 原生函数的签名：(解释器, 参数, 调用行号) -> 返回值。
using NativeFn = std::function<Value(Interpreter&, std::vector<Value>&, int)>;

class NativeCallable : public Callable {
 public:
  NativeCallable(std::string name, NativeFn fn) : name_(std::move(name)), fn_(std::move(fn)) {}

  std::string name() const override { return name_; }
  Value call(Interpreter& in, std::vector<Value>& args, int line) override {
    return fn_(in, args, line);
  }

 private:
  std::string name_;
  NativeFn fn_;
};

struct ListObj {
  std::vector<Value> items;
};

struct DictObj {
  std::map<std::string, Value> items;  // 键统一为字符串
};

inline Value Value::list(std::vector<Value> items) {
  auto obj = std::make_shared<ListObj>();
  obj->items = std::move(items);
  Value v;
  v.data = ListPtr(obj);
  return v;
}

inline Value Value::dict() {
  Value v;
  v.data = DictPtr(std::make_shared<DictObj>());
  return v;
}

// 便捷函数：把 C++ lambda 包装成 dc 可调用的值。
inline Value make_native(std::string name, NativeFn fn) {
  return Value::func(std::make_shared<NativeCallable>(std::move(name), std::move(fn)));
}

// 显示与比较。
std::string format_number(double d);
std::string to_display(const Value& v);  // print 使用的格式
std::string to_repr(const Value& v);     // 字符串带引号，用于容器内部
bool values_equal(const Value& a, const Value& b);

}  // namespace dc

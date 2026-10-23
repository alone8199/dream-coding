// dc/interpreter.hpp
// 树遍历解释器。公共接口分两部分：
//   1. 扩展接口：define_function / define_global / register_module
//   2. 执行接口：run / run_file / call

#pragma once

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "dc/ast.hpp"
#include "dc/value.hpp"

namespace dc {

struct Env;
using EnvPtr = std::shared_ptr<Env>;

// 一层词法作用域。
struct Env {
  explicit Env(EnvPtr parent_env = nullptr) : parent(std::move(parent_env)) {}

  Value* lookup(const std::string& name);
  bool assign(const std::string& name, const Value& value);
  void define(const std::string& name, Value value) { vars[name] = std::move(value); }

  std::unordered_map<std::string, Value> vars;
  EnvPtr parent;
};

// 用 fn 声明的函数，或匿名 fn(...) {...} 表达式，都是 UserFunction。
// closure 保存定义时的环境，所以函数可以捕获外层变量。
class UserFunction : public Callable {
 public:
  UserFunction(NodePtr decl, EnvPtr closure);

  std::string name() const override;
  Value call(Interpreter& in, std::vector<Value>& args, int line) override;

  const NodePtr& decl() const { return decl_; }
  const EnvPtr& closure() const { return closure_; }

 private:
  NodePtr decl_;
  EnvPtr closure_;
};

// 模块工厂：被 import 时调用一次，返回一个字典作为模块对象（结果会被缓存）。
using ModuleFactory = std::function<Value(Interpreter&)>;

class Interpreter {
 public:
  explicit Interpreter(std::ostream& out);

  // ---- 扩展接口 ----

  // 在全局作用域定义一个值（变量或函数）。
  void define_global(const std::string& name, Value value);

  // 在全局作用域定义一个 C++ 函数，脚本里可以直接调用。
  void define_function(const std::string& name, NativeFn fn);

  // 注册一个可以用 import name; 加载的模块。优先级高于同名的 .dc 文件。
  void register_module(const std::string& name, ModuleFactory factory);

  // ---- 执行接口 ----

  // 执行一段源码。出错时抛出 DcError。
  void run(const std::string& source);

  // 执行一个 .dc 文件。文件所在目录会成为 import 的查找目录。
  void run_file(const std::string& path);

  // 调用一个 dc 函数值。
  Value call(const Value& fn, std::vector<Value>& args, int line);

  std::ostream& out() { return out_; }

  // 供 UserFunction 使用。
  Value invoke_user(const UserFunction& fn, std::vector<Value>& args, int line);

 private:
  void exec_stmts(const std::vector<NodePtr>& stmts, const EnvPtr& env);
  void exec(const NodePtr& node, const EnvPtr& env);
  Value eval(const NodePtr& node, const EnvPtr& env);
  Value eval_call(const NodePtr& node, const EnvPtr& env);
  void assign_to(const NodePtr& target, const Value& value, const EnvPtr& env);
  void run_block_top(const std::vector<NodePtr>& stmts, const EnvPtr& env);
  Value import_module(const std::string& spec, int line);
  Value load_file_module(const std::string& spec, int line);

  std::ostream& out_;
  EnvPtr builtins_;
  std::unordered_map<std::string, ModuleFactory> factories_;
  std::unordered_map<std::string, Value> module_cache_;
  std::vector<std::string> import_stack_;
  std::string base_dir_;
  int call_depth_ = 0;
};

// ---- 运算与容器辅助（builtins.cpp 也会用到）----

Value apply_binary(const std::string& op, const Value& l, const Value& r, int line);
Value index_get(const Value& obj, const Value& index, int line);
void index_set(const Value& obj, const Value& index, const Value& value, int line);
std::string dict_key(const Value& key, int line);
std::vector<Value> iterate(const Value& v, int line);

// 列表、字符串、字典的内置方法（如 xs.push(1)）。不认识的方法返回 nullopt。
std::optional<Value> call_method(Interpreter& in, const Value& obj, const std::string& name,
                                 std::vector<Value>& args, int line);

// 内置函数与标准库，分别在 builtins.cpp 和 stdlib.cpp 中定义。
void install_builtins(Interpreter& in);
void install_stdlib(Interpreter& in);

}  // namespace dc

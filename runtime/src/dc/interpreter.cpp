// dc/interpreter.cpp

#include "dc/interpreter.hpp"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "dc/error.hpp"
#include "dc/parser.hpp"

namespace dc {
namespace {

// 调用深度上限。超过就报 "stack overflow in subconscious"。
constexpr int kMaxCallDepth = 200;

// break / continue / return 用异常实现，简单直接。
struct BreakSignal {
  int line;
};
struct ContinueSignal {
  int line;
};
struct ReturnSignal {
  Value value;
  int line;
};

std::string binding_name(const std::string& spec) {
  return std::filesystem::path(spec).stem().string();
}

std::string type_error_text(const std::string& op, const Value& l, const Value& r) {
  return std::string("TypeError: unsupported operand type(s) for '") + op + "': '" +
         l.type_name() + "' and '" + r.type_name() + "'";
}

bool contains(const Value& container, const Value& item, int line) {
  if (container.is_list()) {
    for (const Value& x : std::get<ListPtr>(container.data)->items) {
      if (values_equal(x, item)) return true;
    }
    return false;
  }
  if (container.is_str()) {
    if (!item.is_str()) {
      throw DcError("TypeError: 'in <string>' requires a string on the left", line);
    }
    const auto& hay = std::get<std::string>(container.data);
    return hay.find(std::get<std::string>(item.data)) != std::string::npos;
  }
  if (container.is_dict()) {
    return std::get<DictPtr>(container.data)->items.count(dict_key(item, line)) > 0;
  }
  throw DcError(std::string("TypeError: argument of type '") + container.type_name() +
                    "' is not iterable",
                line);
}

// 把 dc 的数字下标换成合法的 C++ 下标；支持负数从末尾数起。
size_t resolve_index(double d, size_t size, int line) {
  if (d != std::floor(d)) throw DcError("TypeError: index must be an integer", line);
  long long i = static_cast<long long>(d);
  if (i < 0) i += static_cast<long long>(size);
  if (i < 0 || i >= static_cast<long long>(size)) {
    throw DcError("IndexError: index " + format_number(d) + " out of range", line);
  }
  return static_cast<size_t>(i);
}

}  // namespace

// ============================================================
// Env
// ============================================================

Value* Env::lookup(const std::string& name) {
  for (Env* e = this; e != nullptr; e = e->parent.get()) {
    auto it = e->vars.find(name);
    if (it != e->vars.end()) return &it->second;
  }
  return nullptr;
}

bool Env::assign(const std::string& name, const Value& value) {
  if (Value* slot = lookup(name)) {
    *slot = value;
    return true;
  }
  return false;
}

// ============================================================
// UserFunction
// ============================================================

UserFunction::UserFunction(NodePtr decl, EnvPtr closure)
    : decl_(std::move(decl)), closure_(std::move(closure)) {}

std::string UserFunction::name() const {
  return decl_->kind == NodeKind::FnDecl ? decl_->text : "<lambda>";
}

Value UserFunction::call(Interpreter& in, std::vector<Value>& args, int line) {
  return in.invoke_user(*this, args, line);
}

// ============================================================
// 运算与容器辅助
// ============================================================

std::string dict_key(const Value& key, int line) {
  if (key.is_str()) return std::get<std::string>(key.data);
  if (key.is_num()) return format_number(std::get<double>(key.data));
  if (key.is_bool() || key.is_nil()) return to_display(key);
  throw DcError(std::string("TypeError: unhashable type '") + key.type_name() + "'", line);
}

std::vector<Value> iterate(const Value& v, int line) {
  if (v.is_list()) return std::get<ListPtr>(v.data)->items;

  if (v.is_str()) {
    std::vector<Value> out;
    for (char c : std::get<std::string>(v.data)) {
      out.push_back(Value::str(std::string(1, c)));
    }
    return out;
  }

  if (v.is_dict()) {
    std::vector<Value> out;
    for (const auto& [key, value] : std::get<DictPtr>(v.data)->items) {
      out.push_back(Value::str(key));
    }
    return out;
  }

  throw DcError(std::string("TypeError: '") + v.type_name() + "' object is not iterable", line);
}

Value index_get(const Value& obj, const Value& index, int line) {
  if (obj.is_dict()) {
    const auto& items = std::get<DictPtr>(obj.data)->items;
    const std::string key = dict_key(index, line);
    auto it = items.find(key);
    if (it == items.end()) throw DcError("KeyError: " + to_repr(index), line);
    return it->second;
  }

  if (obj.is_list() || obj.is_str()) {
    if (!index.is_num()) throw DcError("TypeError: indices must be numbers", line);
    const double d = std::get<double>(index.data);

    if (obj.is_list()) {
      const auto& items = std::get<ListPtr>(obj.data)->items;
      return items[resolve_index(d, items.size(), line)];
    }
    const auto& s = std::get<std::string>(obj.data);
    return Value::str(std::string(1, s[resolve_index(d, s.size(), line)]));
  }

  throw DcError(std::string("TypeError: '") + obj.type_name() + "' object is not subscriptable",
                line);
}

void index_set(const Value& obj, const Value& index, const Value& value, int line) {
  if (obj.is_dict()) {
    std::get<DictPtr>(obj.data)->items[dict_key(index, line)] = value;
    return;
  }
  if (obj.is_list()) {
    if (!index.is_num()) throw DcError("TypeError: indices must be numbers", line);
    auto& items = std::get<ListPtr>(obj.data)->items;
    items[resolve_index(std::get<double>(index.data), items.size(), line)] = value;
    return;
  }
  throw DcError(std::string("TypeError: '") + obj.type_name() +
                    "' object does not support item assignment",
                line);
}

Value apply_binary(const std::string& op, const Value& l, const Value& r, int line) {
  if (op == "==") return Value::boolean(values_equal(l, r));
  if (op == "!=") return Value::boolean(!values_equal(l, r));
  if (op == "in") return Value::boolean(contains(r, l, line));

  if (op == "+") {
    if (l.is_num() && r.is_num()) {
      return Value::num(std::get<double>(l.data) + std::get<double>(r.data));
    }
    if (l.is_str() && r.is_str()) {
      return Value::str(std::get<std::string>(l.data) + std::get<std::string>(r.data));
    }
    if (l.is_list() && r.is_list()) {
      std::vector<Value> items = std::get<ListPtr>(l.data)->items;
      const auto& more = std::get<ListPtr>(r.data)->items;
      items.insert(items.end(), more.begin(), more.end());
      return Value::list(std::move(items));
    }
    throw DcError(type_error_text(op, l, r), line);
  }

  if (op == "*") {
    if (l.is_num() && r.is_num()) {
      return Value::num(std::get<double>(l.data) * std::get<double>(r.data));
    }
    if (l.is_str() && r.is_num()) {
      const auto& s = std::get<std::string>(l.data);
      const long long times = static_cast<long long>(std::get<double>(r.data));
      std::string out;
      for (long long i = 0; i < times; ++i) out += s;
      return Value::str(std::move(out));
    }
    throw DcError(type_error_text(op, l, r), line);
  }

  if (op == "<" || op == "<=" || op == ">" || op == ">=") {
    int cmp = 0;
    if (l.is_num() && r.is_num()) {
      const double a = std::get<double>(l.data);
      const double b = std::get<double>(r.data);
      cmp = a < b ? -1 : (a > b ? 1 : 0);
    } else if (l.is_str() && r.is_str()) {
      const int c = std::get<std::string>(l.data).compare(std::get<std::string>(r.data));
      cmp = c < 0 ? -1 : (c > 0 ? 1 : 0);
    } else {
      throw DcError(type_error_text(op, l, r), line);
    }
    if (op == "<") return Value::boolean(cmp < 0);
    if (op == "<=") return Value::boolean(cmp <= 0);
    if (op == ">") return Value::boolean(cmp > 0);
    return Value::boolean(cmp >= 0);
  }

  // 减、除、取余只支持数字
  if (!(l.is_num() && r.is_num())) throw DcError(type_error_text(op, l, r), line);
  const double a = std::get<double>(l.data);
  const double b = std::get<double>(r.data);

  if (op == "-") return Value::num(a - b);
  if (op == "/") {
    if (b == 0.0) throw DcError("ZeroDivisionError: division by zero", line);
    return Value::num(a / b);
  }
  if (op == "%") {
    if (b == 0.0) throw DcError("ZeroDivisionError: modulo by zero", line);
    return Value::num(std::fmod(a, b));
  }
  throw DcError("internal error: unknown operator '" + op + "'", line);
}

// ============================================================
// Interpreter：构造与扩展接口
// ============================================================

Interpreter::Interpreter(std::ostream& out) : out_(out), builtins_(std::make_shared<Env>()) {
  install_builtins(*this);
  install_stdlib(*this);
}

void Interpreter::define_global(const std::string& name, Value value) {
  builtins_->define(name, std::move(value));
}

void Interpreter::define_function(const std::string& name, NativeFn fn) {
  define_global(name, make_native(name, std::move(fn)));
}

void Interpreter::register_module(const std::string& name, ModuleFactory factory) {
  factories_[name] = std::move(factory);
}

// ============================================================
// Interpreter：执行接口
// ============================================================

void Interpreter::run(const std::string& source) {
  NodePtr program = parse_program(source);
  auto env = std::make_shared<Env>(builtins_);
  run_block_top(program->kids, env);
}

void Interpreter::run_file(const std::string& path) {
  std::ifstream in(path, std::ios::binary);
  if (!in) throw DcError("FileNotFound: no dream at '" + path + "'", 0);

  std::ostringstream buf;
  buf << in.rdbuf();

  const std::string parent = std::filesystem::path(path).parent_path().string();
  base_dir_ = parent.empty() ? "." : parent;
  run(buf.str());
}

Value Interpreter::call(const Value& fn, std::vector<Value>& args, int line) {
  if (!fn.is_func()) {
    throw DcError(std::string("TypeError: '") + fn.type_name() + "' object is not callable", line);
  }
  if (call_depth_ >= kMaxCallDepth) {
    throw DcError("RecursionError: stack overflow in subconscious", line);
  }
  ++call_depth_;
  struct DepthGuard {
    int& depth;
    ~DepthGuard() { --depth; }
  } guard{call_depth_};

  return std::get<CallPtr>(fn.data)->call(*this, args, line);
}

Value Interpreter::invoke_user(const UserFunction& fn, std::vector<Value>& args, int line) {
  const auto& params = fn.decl()->names;
  if (args.size() != params.size()) {
    throw DcError("TypeError: " + fn.name() + "() takes " + std::to_string(params.size()) +
                      " argument(s) but " + std::to_string(args.size()) + " were given",
                  line);
  }

  auto env = std::make_shared<Env>(fn.closure());
  for (size_t i = 0; i < params.size(); ++i) {
    env->define(params[i], args[i]);
  }

  try {
    exec_stmts(fn.decl()->kids[0]->kids, env);
  } catch (ReturnSignal& ret) {
    return std::move(ret.value);
  }
  return Value::nil();
}

void Interpreter::run_block_top(const std::vector<NodePtr>& stmts, const EnvPtr& env) {
  try {
    exec_stmts(stmts, env);
  } catch (ReturnSignal& s) {
    throw DcError("SyntaxError: 'return' outside function", s.line);
  } catch (BreakSignal& s) {
    throw DcError("SyntaxError: 'break' outside loop", s.line);
  } catch (ContinueSignal& s) {
    throw DcError("SyntaxError: 'continue' outside loop", s.line);
  }
}

// ============================================================
// Interpreter：语句
// ============================================================

void Interpreter::exec_stmts(const std::vector<NodePtr>& stmts, const EnvPtr& env) {
  for (const NodePtr& s : stmts) exec(s, env);
}

void Interpreter::exec(const NodePtr& node, const EnvPtr& env) {
  switch (node->kind) {
    case NodeKind::ExprStmt:
      eval(node->kids[0], env);
      return;

    case NodeKind::Let:
      env->define(node->text, node->kids.empty() ? Value::nil() : eval(node->kids[0], env));
      return;

    case NodeKind::Block: {
      auto inner = std::make_shared<Env>(env);
      exec_stmts(node->kids, inner);
      return;
    }

    case NodeKind::If:
      if (eval(node->kids[0], env).truthy()) {
        exec(node->kids[1], env);
      } else if (node->kids.size() > 2) {
        exec(node->kids[2], env);
      }
      return;

    case NodeKind::While:
      while (eval(node->kids[0], env).truthy()) {
        try {
          exec(node->kids[1], env);
        } catch (BreakSignal&) {
          break;
        } catch (ContinueSignal&) {
          continue;
        }
      }
      return;

    case NodeKind::For: {
      const std::vector<Value> items = iterate(eval(node->kids[0], env), node->line);
      for (const Value& item : items) {
        auto loop_env = std::make_shared<Env>(env);
        loop_env->define(node->text, item);
        try {
          exec(node->kids[1], loop_env);
        } catch (BreakSignal&) {
          break;
        } catch (ContinueSignal&) {
          continue;
        }
      }
      return;
    }

    case NodeKind::Return:
      throw ReturnSignal{node->kids.empty() ? Value::nil() : eval(node->kids[0], env), node->line};

    case NodeKind::Break:
      throw BreakSignal{node->line};

    case NodeKind::Continue:
      throw ContinueSignal{node->line};

    case NodeKind::FnDecl:
      env->define(node->text, Value::func(std::make_shared<UserFunction>(node, env)));
      return;

    case NodeKind::Import: {
      Value module = import_module(node->text, node->line);
      env->define(binding_name(node->text), std::move(module));
      return;
    }

    default:
      throw DcError("internal error: unexpected statement", node->line);
  }
}

// ============================================================
// Interpreter：表达式
// ============================================================

Value Interpreter::eval(const NodePtr& node, const EnvPtr& env) {
  switch (node->kind) {
    case NodeKind::Number:
      return Value::num(node->number);

    case NodeKind::String:
      return Value::str(node->text);

    case NodeKind::Bool:
      return Value::boolean(node->number != 0.0);

    case NodeKind::Null:
      return Value::nil();

    case NodeKind::Ident: {
      Value* found = env->lookup(node->text);
      if (found == nullptr) {
        throw DcError("NameError: name '" + node->text + "' is not defined", node->line);
      }
      return *found;
    }

    case NodeKind::ListLit: {
      std::vector<Value> items;
      items.reserve(node->kids.size());
      for (const NodePtr& k : node->kids) items.push_back(eval(k, env));
      return Value::list(std::move(items));
    }

    case NodeKind::DictLit: {
      Value d = Value::dict();
      auto& items = std::get<DictPtr>(d.data)->items;
      for (size_t i = 0; i + 1 < node->kids.size(); i += 2) {
        Value key = eval(node->kids[i], env);
        items[dict_key(key, node->line)] = eval(node->kids[i + 1], env);
      }
      return d;
    }

    case NodeKind::FnExpr:
      return Value::func(std::make_shared<UserFunction>(node, env));

    case NodeKind::Unary: {
      Value v = eval(node->kids[0], env);
      if (node->text == "-") {
        if (!v.is_num()) {
          throw DcError(std::string("TypeError: bad operand type for unary -: '") +
                            v.type_name() + "'",
                        node->line);
        }
        return Value::num(-std::get<double>(v.data));
      }
      return Value::boolean(!v.truthy());
    }

    case NodeKind::Logical: {
      Value left = eval(node->kids[0], env);
      if (node->text == "or") {
        return left.truthy() ? left : eval(node->kids[1], env);
      }
      return !left.truthy() ? left : eval(node->kids[1], env);
    }

    case NodeKind::Binary: {
      Value left = eval(node->kids[0], env);
      Value right = eval(node->kids[1], env);
      return apply_binary(node->text, left, right, node->line);
    }

    case NodeKind::Assign: {
      Value value = eval(node->kids[1], env);
      assign_to(node->kids[0], value, env);
      return value;
    }

    case NodeKind::Call:
      return eval_call(node, env);

    case NodeKind::Index: {
      Value obj = eval(node->kids[0], env);
      Value index = eval(node->kids[1], env);
      return index_get(obj, index, node->line);
    }

    case NodeKind::Member: {
      Value obj = eval(node->kids[0], env);
      if (obj.is_dict()) {
        const auto& items = std::get<DictPtr>(obj.data)->items;
        auto it = items.find(node->text);
        if (it != items.end()) return it->second;
      }
      throw DcError(std::string("AttributeError: '") + obj.type_name() +
                        "' object has no attribute '" + node->text + "'",
                    node->line);
    }

    default:
      throw DcError("internal error: unexpected expression", node->line);
  }
}

Value Interpreter::eval_call(const NodePtr& node, const EnvPtr& env) {
  const NodePtr& callee = node->kids[0];
  std::vector<Value> args;
  Value fn;

  if (callee->kind == NodeKind::Member) {
    Value target = eval(callee->kids[0], env);

    // 先看字典里有没有这个字段（模块的函数就是这样来的）
    bool is_field = false;
    if (target.is_dict()) {
      const auto& items = std::get<DictPtr>(target.data)->items;
      auto it = items.find(callee->text);
      if (it != items.end()) {
        fn = it->second;
        is_field = true;
      }
    }

    if (!is_field) {
      for (size_t i = 1; i < node->kids.size(); ++i) {
        args.push_back(eval(node->kids[i], env));
      }
      if (auto result = call_method(*this, target, callee->text, args, node->line)) {
        return *result;
      }
      throw DcError(std::string("AttributeError: '") + target.type_name() +
                        "' object has no attribute '" + callee->text + "'",
                    node->line);
    }
  } else {
    fn = eval(callee, env);
  }

  for (size_t i = 1; i < node->kids.size(); ++i) {
    args.push_back(eval(node->kids[i], env));
  }
  return call(fn, args, node->line);
}

void Interpreter::assign_to(const NodePtr& target, const Value& value, const EnvPtr& env) {
  switch (target->kind) {
    case NodeKind::Ident:
      if (!env->assign(target->text, value)) {
        throw DcError("NameError: name '" + target->text + "' is not defined (use let first)",
                      target->line);
      }
      return;

    case NodeKind::Index: {
      Value obj = eval(target->kids[0], env);
      Value index = eval(target->kids[1], env);
      index_set(obj, index, value, target->line);
      return;
    }

    case NodeKind::Member: {
      Value obj = eval(target->kids[0], env);
      if (!obj.is_dict()) {
        throw DcError(std::string("AttributeError: '") + obj.type_name() +
                          "' object does not support attribute assignment",
                      target->line);
      }
      std::get<DictPtr>(obj.data)->items[target->text] = value;
      return;
    }

    default:
      throw DcError("SyntaxError: invalid assignment target", target->line);
  }
}

// ============================================================
// Interpreter：导入
// ============================================================

Value Interpreter::import_module(const std::string& spec, int line) {
  if (auto cached = module_cache_.find(spec); cached != module_cache_.end()) {
    return cached->second;
  }
  if (auto factory = factories_.find(spec); factory != factories_.end()) {
    Value module = factory->second(*this);
    module_cache_[spec] = module;
    return module;
  }
  return load_file_module(spec, line);
}

Value Interpreter::load_file_module(const std::string& spec, int line) {
  namespace fs = std::filesystem;

  const bool has_ext = spec.size() > 3 && spec.compare(spec.size() - 3, 3, ".dc") == 0;
  const fs::path dir = base_dir_.empty() ? fs::path(".") : fs::path(base_dir_);
  const fs::path file = dir / (has_ext ? spec : spec + ".dc");

  if (!fs::exists(file)) {
    throw DcError("ImportError: no module named '" + spec + "'", line);
  }

  const std::string key = fs::weakly_canonical(file).string();
  if (auto cached = module_cache_.find(key); cached != module_cache_.end()) {
    return cached->second;
  }
  if (std::find(import_stack_.begin(), import_stack_.end(), key) != import_stack_.end()) {
    throw DcError("ImportError: circular import of '" + spec + "'", line);
  }

  std::ifstream in(file, std::ios::binary);
  std::ostringstream buf;
  buf << in.rdbuf();
  NodePtr program = parse_program(buf.str());

  const std::string saved_dir = base_dir_;
  base_dir_ = file.parent_path().string();
  import_stack_.push_back(key);

  auto env = std::make_shared<Env>(builtins_);
  try {
    run_block_top(program->kids, env);
  } catch (...) {
    import_stack_.pop_back();
    base_dir_ = saved_dir;
    throw;
  }
  import_stack_.pop_back();
  base_dir_ = saved_dir;

  // 模块的导出：所有不以 _ 开头的顶层名字
  Value module = Value::dict();
  auto& items = std::get<DictPtr>(module.data)->items;
  for (const auto& [name, value] : env->vars) {
    if (!name.empty() && name[0] != '_') items[name] = value;
  }
  module_cache_[key] = module;
  return module;
}

}  // namespace dc

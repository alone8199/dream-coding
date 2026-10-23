// dc/ast.hpp
// 语法树。为了让扩展更简单，所有节点共用一个结构体，靠 kind 区分含义。

#pragma once

#include <memory>
#include <string>
#include <vector>

namespace dc {

enum class NodeKind {
  // 表达式
  Number,    // number
  String,    // text
  Bool,      // number: 1 或 0
  Null,
  Ident,     // text
  ListLit,   // kids: 元素
  DictLit,   // kids: 键1, 值1, 键2, 值2, ...
  FnExpr,    // names: 参数; kids[0]: 函数体
  Unary,     // text: "-" 或 "not"; kids[0]
  Binary,    // text: 运算符; kids[0], kids[1]
  Logical,   // text: "and" 或 "or"; kids[0], kids[1]（短路求值）
  Assign,    // kids[0]: 目标（Ident / Index / Member）; kids[1]: 值
  Call,      // kids[0]: 被调用者; kids[1..]: 参数
  Index,     // kids[0]: 对象; kids[1]: 索引
  Member,    // kids[0]: 对象; text: 成员名
  // 语句
  ExprStmt,  // kids[0]
  Let,       // text: 名字; kids[0]: 初始值
  Block,     // kids: 语句
  If,        // kids: 条件, 分支, [else 分支]
  While,     // kids: 条件, 循环体
  For,       // text: 循环变量; kids: 可迭代对象, 循环体
  Return,    // kids: [返回值]
  Break,
  Continue,
  FnDecl,    // text: 名字; names: 参数; kids[0]: 函数体
  Import,    // text: 模块名或路径
};

struct Node;
using NodePtr = std::shared_ptr<Node>;

struct Node {
  NodeKind kind = NodeKind::Null;
  int line = 1;
  std::string text;
  double number = 0.0;
  std::vector<std::string> names;
  std::vector<NodePtr> kids;
};

}  // namespace dc

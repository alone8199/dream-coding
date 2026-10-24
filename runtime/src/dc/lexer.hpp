// dc/lexer.hpp
// 把源码切成记号（token）。

#pragma once

#include <string>
#include <vector>

namespace dc {

enum class Tok {
  End,
  Ident,
  Number,
  String,
  // 关键字
  KwLet,
  KwFn,
  KwReturn,
  KwIf,
  KwElse,
  KwWhile,
  KwFor,
  KwIn,
  KwBreak,
  KwContinue,
  KwImport,
  KwTrue,
  KwFalse,
  KwNull,
  KwAnd,
  KwOr,
  KwNot,
  // 符号
  LParen,
  RParen,
  LBrace,
  RBrace,
  LBracket,
  RBracket,
  Comma,
  Semicolon,
  Colon,
  Dot,
  Assign,
  Eq,
  NotEq,
  Less,
  LessEq,
  Greater,
  GreaterEq,
  Plus,
  Minus,
  Star,
  Slash,
  Percent,
  Bang,
  AndAnd,
  OrOr,
};

struct Token {
  Tok kind;
  std::string text;  // 标识符、符号或字符串的内容（字符串已去掉引号并解转义）
  double number;     // 仅数字记号有效
  int line;
};

// 切分源码。遇到非法字符或未闭合字符串时抛出 DcError（SyntaxError）。
std::vector<Token> tokenize(const std::string& source);

}  // namespace dc

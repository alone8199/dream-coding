// dc/parser.cpp
// 递归下降语法分析。
// 优先级从低到高：赋值 > 或 > 与 > 相等 > 比较(含 in) > 加减 > 乘除 > 一元 > 后缀 > 基本项。
// 分号可以省略；字典键可以直接写名字，例如 {name: "dc"}。

#include "dc/parser.hpp"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "dc/error.hpp"
#include "dc/lexer.hpp"

namespace dc {
namespace {

NodePtr make(NodeKind kind, int line) {
  auto n = std::make_shared<Node>();
  n->kind = kind;
  n->line = line;
  return n;
}

NodePtr binary(NodePtr left, const std::string& op, NodePtr right, NodeKind k) {
  auto n = make(k, left->line);
  n->text = op;
  n->kids.push_back(std::move(left));
  n->kids.push_back(std::move(right));
  return n;
}

class Parser {
 public:
  explicit Parser(std::vector<Token> tokens) : toks_(std::move(tokens)) {}

  NodePtr program() {
    auto block = make(NodeKind::Block, 1);
    while (kind() != Tok::End) {
      block->kids.push_back(statement());
    }
    return block;
  }

 private:
  std::vector<Token> toks_;
  size_t pos_ = 0;

  // ---- 记号工具 ----

  const Token& peek(size_t ahead = 0) const {
    return toks_[std::min(pos_ + ahead, toks_.size() - 1)];
  }

  Tok kind(size_t ahead = 0) const { return peek(ahead).kind; }

  const Token& advance() {
    const Token& t = toks_[pos_];
    if (pos_ + 1 < toks_.size()) ++pos_;
    return t;
  }

  bool match(Tok k) {
    if (kind() == k) {
      advance();
      return true;
    }
    return false;
  }

  const Token& expect(Tok k, const char* what) {
    if (kind() != k) {
      fail(std::string("expected ") + what + ", found " + describe(peek()));
    }
    return advance();
  }

  std::string expect_name(const char* what = "a name") {
    if (kind() != Tok::Ident) {
      fail(std::string("expected ") + what + ", found " + describe(peek()));
    }
    return advance().text;
  }

  [[noreturn]] void fail(const std::string& msg) const {
    throw DcError("SyntaxError: " + msg, peek().line);
  }

  static std::string describe(const Token& t) {
    if (t.kind == Tok::End) return "end of file";
    if (t.kind == Tok::String) return "string \"" + t.text + "\"";
    if (t.kind == Tok::Number) return "number " + t.text;
    return "'" + t.text + "'";
  }

  void end_statement() { match(Tok::Semicolon); }

  // ---- 语句 ----

  NodePtr statement() {
    const int line = peek().line;

    switch (kind()) {
      case Tok::KwLet: {
        advance();
        auto n = make(NodeKind::Let, line);
        n->text = expect_name();
        expect(Tok::Assign, "'='");
        n->kids.push_back(expression());
        end_statement();
        return n;
      }
      case Tok::KwFn:
        if (kind(1) == Tok::Ident) {
          advance();
          return fn_decl(line);
        }
        break;
      case Tok::KwIf:
        return if_statement();
      case Tok::KwWhile: {
        advance();
        auto n = make(NodeKind::While, line);
        n->kids.push_back(expression());
        n->kids.push_back(block());
        return n;
      }
      case Tok::KwFor: {
        advance();
        auto n = make(NodeKind::For, line);
        n->text = expect_name("loop variable");
        expect(Tok::KwIn, "'in'");
        n->kids.push_back(expression());
        n->kids.push_back(block());
        return n;
      }
      case Tok::KwReturn: {
        advance();
        auto n = make(NodeKind::Return, line);
        if (kind() != Tok::Semicolon && kind() != Tok::RBrace && kind() != Tok::End) {
          n->kids.push_back(expression());
        }
        end_statement();
        return n;
      }
      case Tok::KwBreak:
        advance();
        end_statement();
        return make(NodeKind::Break, line);
      case Tok::KwContinue:
        advance();
        end_statement();
        return make(NodeKind::Continue, line);
      case Tok::KwImport: {
        advance();
        auto n = make(NodeKind::Import, line);
        if (kind() == Tok::String) {
          n->text = advance().text;  // import "lib/util.dc";
        } else {
          n->text = expect_name("module name");  // import math;
        }
        end_statement();
        return n;
      }
      case Tok::LBrace:
        return block();
      default:
        break;
    }

    auto n = make(NodeKind::ExprStmt, line);
    n->kids.push_back(expression());
    end_statement();
    return n;
  }

  // "fn" 已经消耗，名字在当前位置。
  NodePtr fn_decl(int line) {
    auto n = make(NodeKind::FnDecl, line);
    n->text = expect_name("function name");
    n->names = params();
    n->kids.push_back(block());
    return n;
  }

  std::vector<std::string> params() {
    expect(Tok::LParen, "'('");
    std::vector<std::string> names;
    if (kind() != Tok::RParen) {
      do {
        names.push_back(expect_name("parameter name"));
      } while (match(Tok::Comma));
    }
    expect(Tok::RParen, "')'");
    return names;
  }

  NodePtr if_statement() {
    const int line = advance().line;  // 'if'
    auto n = make(NodeKind::If, line);
    n->kids.push_back(expression());
    n->kids.push_back(block());
    if (match(Tok::KwElse)) {
      n->kids.push_back(kind() == Tok::KwIf ? if_statement() : block());
    }
    return n;
  }

  NodePtr block() {
    const int line = expect(Tok::LBrace, "'{'").line;
    auto n = make(NodeKind::Block, line);
    while (kind() != Tok::RBrace) {
      if (kind() == Tok::End) {
        fail("missing '}' to close the block opened on line " + std::to_string(line));
      }
      n->kids.push_back(statement());
    }
    advance();  // '}'
    return n;
  }

  // ---- 表达式 ----

  NodePtr expression() { return assignment(); }

  NodePtr assignment() {
    NodePtr lhs = logic_or();
    if (kind() != Tok::Assign) return lhs;

    const int line = advance().line;
    if (lhs->kind != NodeKind::Ident && lhs->kind != NodeKind::Index &&
        lhs->kind != NodeKind::Member) {
      throw DcError("SyntaxError: invalid assignment target", line);
    }
    auto n = make(NodeKind::Assign, line);
    n->kids.push_back(std::move(lhs));
    n->kids.push_back(assignment());
    return n;
  }

  NodePtr logic_or() {
    NodePtr left = logic_and();
    while (kind() == Tok::KwOr || kind() == Tok::OrOr) {
      advance();
      left = binary(std::move(left), "or", logic_and(), NodeKind::Logical);
    }
    return left;
  }

  NodePtr logic_and() {
    NodePtr left = equality();
    while (kind() == Tok::KwAnd || kind() == Tok::AndAnd) {
      advance();
      left = binary(std::move(left), "and", equality(), NodeKind::Logical);
    }
    return left;
  }

  NodePtr equality() {
    NodePtr left = comparison();
    while (kind() == Tok::Eq || kind() == Tok::NotEq) {
      const std::string op = advance().text;
      left = binary(std::move(left), op, comparison(), NodeKind::Binary);
    }
    return left;
  }

  NodePtr comparison() {
    NodePtr left = additive();
    while (kind() == Tok::Less || kind() == Tok::LessEq || kind() == Tok::Greater ||
           kind() == Tok::GreaterEq || kind() == Tok::KwIn) {
      std::string op = advance().text;
      left = binary(std::move(left), op, additive(), NodeKind::Binary);
    }
    return left;
  }

  NodePtr additive() {
    NodePtr left = multiplicative();
    while (kind() == Tok::Plus || kind() == Tok::Minus) {
      const std::string op = advance().text;
      left = binary(std::move(left), op, multiplicative(), NodeKind::Binary);
    }
    return left;
  }

  NodePtr multiplicative() {
    NodePtr left = unary();
    while (kind() == Tok::Star || kind() == Tok::Slash || kind() == Tok::Percent) {
      const std::string op = advance().text;
      left = binary(std::move(left), op, unary(), NodeKind::Binary);
    }
    return left;
  }

  NodePtr unary() {
    if (kind() == Tok::Minus || kind() == Tok::Bang || kind() == Tok::KwNot) {
      const Token& t = advance();
      auto n = make(NodeKind::Unary, t.line);
      n->text = (t.kind == Tok::Minus) ? "-" : "not";
      n->kids.push_back(unary());
      return n;
    }
    return postfix();
  }

  NodePtr postfix() {
    NodePtr e = primary();
    while (true) {
      const int line = peek().line;
      if (match(Tok::LParen)) {
        auto call = make(NodeKind::Call, line);
        call->kids.push_back(std::move(e));
        if (kind() != Tok::RParen) {
          do {
            call->kids.push_back(expression());
          } while (match(Tok::Comma));
        }
        expect(Tok::RParen, "')'");
        e = std::move(call);
      } else if (match(Tok::LBracket)) {
        auto index = make(NodeKind::Index, line);
        index->kids.push_back(std::move(e));
        index->kids.push_back(expression());
        expect(Tok::RBracket, "']'");
        e = std::move(index);
      } else if (match(Tok::Dot)) {
        auto member = make(NodeKind::Member, line);
        member->kids.push_back(std::move(e));
        member->text = expect_name("member name");
        e = std::move(member);
      } else {
        return e;
      }
    }
  }

  NodePtr primary() {
    const Token& t = peek();
    const int line = t.line;

    switch (t.kind) {
      case Tok::Number: {
        auto n = make(NodeKind::Number, line);
        n->number = t.number;
        advance();
        return n;
      }
      case Tok::String: {
        auto n = make(NodeKind::String, line);
        n->text = t.text;
        advance();
        return n;
      }
      case Tok::KwTrue:
      case Tok::KwFalse: {
        auto n = make(NodeKind::Bool, line);
        n->number = (t.kind == Tok::KwTrue) ? 1.0 : 0.0;
        advance();
        return n;
      }
      case Tok::KwNull:
        advance();
        return make(NodeKind::Null, line);
      case Tok::Ident: {
        auto n = make(NodeKind::Ident, line);
        n->text = t.text;
        advance();
        return n;
      }
      case Tok::KwFn: {
        advance();
        auto n = make(NodeKind::FnExpr, line);
        n->names = params();
        n->kids.push_back(block());
        return n;
      }
      case Tok::LParen: {
        advance();
        NodePtr e = expression();
        expect(Tok::RParen, "')'");
        return e;
      }
      case Tok::LBracket: {
        advance();
        auto n = make(NodeKind::ListLit, line);
        while (kind() != Tok::RBracket) {
          n->kids.push_back(expression());
          if (!match(Tok::Comma)) break;
        }
        expect(Tok::RBracket, "']'");
        return n;
      }
      case Tok::LBrace: {
        advance();
        auto n = make(NodeKind::DictLit, line);
        while (kind() != Tok::RBrace) {
          if (kind() == Tok::Ident && kind(1) == Tok::Colon) {
            auto key = make(NodeKind::String, peek().line);
            key->text = advance().text;
            n->kids.push_back(key);
          } else {
            n->kids.push_back(expression());
          }
          expect(Tok::Colon, "':'");
          n->kids.push_back(expression());
          if (!match(Tok::Comma)) break;
        }
        expect(Tok::RBrace, "'}'");
        return n;
      }
      default:
        fail("unexpected " + describe(t));
    }
  }
};

}  // namespace

NodePtr parse_program(const std::string& source) {
  Parser parser(tokenize(source));
  return parser.program();
}

}  // namespace dc

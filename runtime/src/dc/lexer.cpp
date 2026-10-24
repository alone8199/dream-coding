// dc/lexer.cpp

#include "dc/lexer.hpp"

#include <cctype>
#include <cstdlib>
#include <unordered_map>

#include "dc/error.hpp"

namespace dc {
namespace {

const std::unordered_map<std::string, Tok> kKeywords = {
    {"let", Tok::KwLet},         {"fn", Tok::KwFn},         {"return", Tok::KwReturn},
    {"if", Tok::KwIf},           {"else", Tok::KwElse},     {"while", Tok::KwWhile},
    {"for", Tok::KwFor},         {"in", Tok::KwIn},         {"break", Tok::KwBreak},
    {"continue", Tok::KwContinue}, {"import", Tok::KwImport}, {"true", Tok::KwTrue},
    {"false", Tok::KwFalse},     {"null", Tok::KwNull},     {"and", Tok::KwAnd},
    {"or", Tok::KwOr},           {"not", Tok::KwNot},
};

const std::unordered_map<std::string, Tok> kTwoChar = {
    {"==", Tok::Eq},     {"!=", Tok::NotEq},       {"<=", Tok::LessEq},
    {">=", Tok::GreaterEq}, {"&&", Tok::AndAnd},   {"||", Tok::OrOr},
};

bool is_ident_start(unsigned char c) {
  // 字节 >= 0x80 也允许作为标识符，方便使用中文变量名
  return std::isalpha(c) || c == '_' || c >= 0x80;
}

bool is_ident_char(unsigned char c) {
  return is_ident_start(c) || std::isdigit(c);
}

Tok single_char_token(char c, bool& ok) {
  ok = true;
  switch (c) {
    case '(': return Tok::LParen;
    case ')': return Tok::RParen;
    case '{': return Tok::LBrace;
    case '}': return Tok::RBrace;
    case '[': return Tok::LBracket;
    case ']': return Tok::RBracket;
    case ',': return Tok::Comma;
    case ';': return Tok::Semicolon;
    case ':': return Tok::Colon;
    case '.': return Tok::Dot;
    case '=': return Tok::Assign;
    case '<': return Tok::Less;
    case '>': return Tok::Greater;
    case '+': return Tok::Plus;
    case '-': return Tok::Minus;
    case '*': return Tok::Star;
    case '/': return Tok::Slash;
    case '%': return Tok::Percent;
    case '!': return Tok::Bang;
    default: ok = false; return Tok::End;
  }
}

}  // namespace

std::vector<Token> tokenize(const std::string& src) {
  std::vector<Token> out;
  int line = 1;
  size_t i = 0;
  const size_t n = src.size();

  auto push = [&](Tok kind, std::string text, double num = 0.0) {
    out.push_back(Token{kind, std::move(text), num, line});
  };

  while (i < n) {
    const char c = src[i];

    if (c == '\n') {
      ++line;
      ++i;
      continue;
    }
    if (std::isspace(static_cast<unsigned char>(c))) {
      ++i;
      continue;
    }
    // 注释：# 与 // 都可以，直到行尾
    if (c == '#' || (c == '/' && i + 1 < n && src[i + 1] == '/')) {
      while (i < n && src[i] != '\n') ++i;
      continue;
    }

    if (is_ident_start(static_cast<unsigned char>(c))) {
      const size_t start = i;
      while (i < n && is_ident_char(static_cast<unsigned char>(src[i]))) ++i;
      std::string word = src.substr(start, i - start);
      auto it = kKeywords.find(word);
      push(it == kKeywords.end() ? Tok::Ident : it->second, std::move(word));
      continue;
    }

    if (std::isdigit(static_cast<unsigned char>(c))) {
      const size_t start = i;
      while (i < n && std::isdigit(static_cast<unsigned char>(src[i]))) ++i;
      if (i + 1 < n && src[i] == '.' && std::isdigit(static_cast<unsigned char>(src[i + 1]))) {
        ++i;
        while (i < n && std::isdigit(static_cast<unsigned char>(src[i]))) ++i;
      }
      std::string text = src.substr(start, i - start);
      push(Tok::Number, text, std::strtod(text.c_str(), nullptr));
      continue;
    }

    if (c == '"') {
      const int start_line = line;
      ++i;
      std::string value;
      while (true) {
        if (i >= n || src[i] == '\n') {
          throw DcError("SyntaxError: unterminated string", start_line);
        }
        const char d = src[i++];
        if (d == '"') break;
        if (d != '\\') {
          value += d;
          continue;
        }
        if (i >= n) throw DcError("SyntaxError: unterminated escape", line);
        const char e = src[i++];
        switch (e) {
          case 'n': value += '\n'; break;
          case 't': value += '\t'; break;
          case 'r': value += '\r'; break;
          case '0': value += '\0'; break;
          case '"': value += '"'; break;
          case '\\': value += '\\'; break;
          default:
            throw DcError(std::string("SyntaxError: unknown escape '\\") + e + "'", line);
        }
      }
      out.push_back(Token{Tok::String, std::move(value), 0.0, start_line});
      continue;
    }

    if (i + 1 < n) {
      const std::string two = src.substr(i, 2);
      auto it = kTwoChar.find(two);
      if (it != kTwoChar.end()) {
        push(it->second, two);
        i += 2;
        continue;
      }
    }

    bool ok = false;
    const Tok single = single_char_token(c, ok);
    if (ok) {
      push(single, std::string(1, c));
      ++i;
      continue;
    }

    throw DcError(std::string("SyntaxError: unexpected character '") + c + "'", line);
  }

  push(Tok::End, "");
  return out;
}

}  // namespace dc

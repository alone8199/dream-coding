// dc/error.hpp
// 所有 dc 错误（语法、运行时、导入）都用这一个类型表示。

#pragma once

#include <stdexcept>
#include <string>

namespace dc {

class DcError : public std::runtime_error {
 public:
  DcError(const std::string& message, int line)
      : std::runtime_error(message), line_(line) {}

  int line() const { return line_; }

 private:
  int line_;
};

}  // namespace dc

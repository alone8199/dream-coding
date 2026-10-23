// dc/parser.hpp
// 语法分析：递归下降，生成语法树。

#pragma once

#include <string>

#include "dc/ast.hpp"

namespace dc {

// 解析整个源文件，返回一个 Block 节点。
// 出错时抛出 DcError，消息以 "SyntaxError:" 开头。
NodePtr parse_program(const std::string& source);

}  // namespace dc

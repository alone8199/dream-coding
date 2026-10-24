# 20 Lexer Design

The lexer is the first step in how `dc` processes source code. Its job is to cut a string into a sequence of tokens, where each token carries a kind, its text, and the line it sits on. This step does not care whether the syntax is correct. It only cares which category each fragment belongs to.

The tokens of `dc` fall into several categories. The first is identifiers and keywords. An identifier is made of letters, underscores, and digits, but it cannot begin with a digit. Keywords such as `let`, `fn`, `if`, `while`, and `import` are recognized as dedicated token kinds, not as ordinary identifiers. The second category is numbers, which may be integers or decimals; both are represented uniformly as double-precision floating-point values. The third is strings, which are enclosed in double quotes and support escape sequences such as `\n`, `\t`, and `\"`. The fourth is symbols, which include parentheses, commas, semicolons, dots, and the various operators.

A few details deserve separate mention. First, identifiers may contain non-ASCII characters. The lexer treats every character whose byte value is greater than or equal to 0x80 as part of an identifier, which makes Chinese variable names possible. The cost of this design is that the lexer does no Unicode normalization of variable names, but for dream coding, such a simple rule is already enough.

Second, comments can be written in two ways: starting with a hash sign, or starting with a double slash, and running to the end of the line. Supporting both is meant to let developers with different habits write code comfortably. Comments are skipped directly during lexing and produce no tokens at all.

Third, operators are recognized in two steps. The lexer first checks for two-character operators, such as equal-to, not-equal, less-than-or-equal, and the logical and and logical or. If nothing matches, it checks for single-character operators. This guarantees that `<=` is recognized as a single unit, not as the combination of `<` and `=`.

Fourth, a line break inside a string is not allowed. If the lexer encounters an unclosed string, it reports a syntax error immediately and points to the line where the string began. This strict rule makes errors surface early, instead of producing hard-to-understand chain reactions in later statements.

The lexer's output is an array, and its last element is always the token that marks the end of the file. This design lets the parser look ahead safely without worrying about running past the end.

The whole lexer is very straightforward. It makes a single left-to-right scan, with no backtracking and no state-machine logic beyond what is needed. This simplicity makes it easy to test and maintain, and it is one of the reasons `dc` can be built quickly on every platform.

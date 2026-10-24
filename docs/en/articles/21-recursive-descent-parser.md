# 21 Recursive Descent Parsing

The job of the parser is to turn a token sequence into a syntax tree. `dc` implements it using recursive descent. The defining feature of this approach is that each kind of grammar construct corresponds to one function, and the calls between the functions mirror the nesting of the grammar. It is intuitive, easy to read, and easy to debug.

The entry point of the parser is a program function. It keeps reading statements until it reaches the token that marks the end of the file, and then collects all the statements into a block node. The block node is the root of the whole program, and its children are the top-level statements.

Statement parsing is handled by a dispatch function. Based on the kind of the current token, it decides which sub-function to call. On seeing `let`, it parses a `let` statement. On seeing `if`, it parses a conditional. On seeing `fn` followed by a name, it parses a function declaration. On seeing an opening brace, it parses a block. If none of these match, it treats the input as an expression statement. This way of dispatching keeps the rules for each kind of statement independent.

Expression parsing is the most complicated part. `dc` implements, from lowest precedence to highest, assignment, logical or, logical and, equality, comparison, addition and subtraction, multiplication and division, unary operators, postfix operators, and primary terms. Each layer of functions is responsible only for its own precedence level. When it encounters an operator of higher precedence, it calls the next layer down. This layering ensures that multiplication is evaluated before addition, and that the logical operators have the lowest precedence.

Assignment is a special case. The left-hand side must be a valid target, such as a variable, an index, or a member access. The parser first parses the left-hand side as an ordinary expression, then checks its kind. If it is not a valid target, it reports a syntax error. This check catches mistakes at parse time instead of leaving them until runtime.

Postfix operations include function calls, indexing, and member access. They can appear one after another, as in `obj.items[0].name()`. The parser handles them with a loop: each time it meets a postfix, it wraps the expression so far in a new node. This approach avoids the complications of recursion while preserving the correct order of association.

One advantage of recursive descent is that error messages are easy to pin down. When the parser meets an unexpected token, it knows which grammar rule it is currently inside, so it can give a specific hint, such as `SyntaxError: expected ')', found ';'`. A message like that is far more useful than a generic "syntax error".

It should be noted that recursive descent also has limits. It has trouble with left-recursive grammars, which is why `dc` implements its operators with loops rather than recursion. It is also sensitive to nesting depth, and an overly deep expression may overflow the stack. For dream scripts, however, these cases are very rare.

In short, recursive descent is a plain but reliable method. It lets every line of the parser correspond to a grammar rule, and that correspondence is itself the best documentation for understanding the syntax of `dc`.

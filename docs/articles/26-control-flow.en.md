# 26 Control Flow: Implementing break, continue, and return

[中文](./26-control-flow.md) | [English](./26-control-flow.en.md)

Control flow determines the order in which a program executes. `dc` provides three structures, `if`, `while`, and `for`, along with three jump statements, `break`, `continue`, and `return`. The meaning of the first three is intuitive, while the way the last three are implemented deserves separate discussion.

An `if` statement chooses which branch to run based on whether a condition is true or false. The condition uses the general truth rules, so an empty list and zero are both treated as false. An `if` can have an `else`, and it can chain `else if` to form a multi-way branch. In the evaluator, these structures map directly onto conditional checks, with no special complexity.

A `while` statement runs its body repeatedly as long as the condition is true. Before each iteration begins, the evaluator recomputes the condition. A `for` statement walks over an iterable object, which may be a list, a string, or a dictionary. A string is traversed byte by byte. For non-ASCII text such as Chinese, one character may occupy several bytes, so you need to keep this in mind. A dictionary is traversed key by key. The loop variable of a `for` is a new binding on every iteration, a point we already mentioned in the article on environments.

The implementation of `break` and `continue` relies on C++ exception handling. When the evaluator executes a `break`, it throws a special exception object. The loop's execution code catches the exception and ends the loop. `continue` works similarly, except that after catching the exception, it skips the rest of the current iteration and moves on to the next one. The benefit of this implementation is that its logic is clear: the intent to jump corresponds directly to throwing and catching an exception, with no need to check flags manually at every level of statement.

The implementation of `return` also uses exceptions. When the evaluator executes a `return`, it throws an exception that carries the return value. The code that calls the function catches the exception and takes out the return value as the result of the call. If the function body ends normally without meeting a `return`, the result of the call is nil.

Using exceptions for jumps has one cost to keep in mind: throwing and catching an exception is much slower than an ordinary function return. For dream scripts, the number of loop iterations and function calls is usually not large, so this cost is acceptable. If a future need arises to handle large numbers of loops, one could switch to returning status codes instead.

`break`, `continue`, and `return` are all subject to scope restrictions. Using `break` or `continue` outside a loop is a syntax error, and so is using `return` outside a function. These checks are carried out at runtime, and the error message points to the specific line, helping the developer find the problem quickly.

It should be stated that the rules above for `break` and `continue` do not fully hold inside a function body yet. A `break` or `continue` inside a function body is not rejected yet; the jump crosses the function boundary and affects the loop where the caller is running. This is a known defect in the current version, and it is planned to be fixed in a later version.

A special reminder: the exception mechanism does not cross module boundaries. If a `return` appears in a module's top-level code, it is converted into a syntax error, rather than making the importer's function return early. This ensures that running a module cannot accidentally affect the control flow of the outside.

In short, the control flow of `dc` is built around simplicity. It uses exceptions to express jumps, environments to express scope, and recursion to execute statements. These choices let each part of the code correspond directly to a semantic of the language, and they let the developer quickly understand the execution path of a script.

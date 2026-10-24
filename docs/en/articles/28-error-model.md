# 28 The Error Model: DcError with Line Numbers

Error handling determines how a program reports problems when something goes wrong. The error model of `dc` is simple: every error is represented by the same exception type, `DcError`, which carries the error message and the line number where the error occurred. Whether the error happens during parsing, evaluation or importing, it is represented in the same way.

`DcError` inherits from the standard library's runtime error type, so it can be caught and handled like any other C++ exception. It also stores the line number, which is essential for error reporting. Developers need to know which line of the script the error came from in order to locate the problem quickly.

Error messages follow a consistent convention: a prefix names the category of the error, followed by a specific description. For example, `NameError: name 'zzz' is not defined` means a name is not defined, `TypeError` means a type mismatch, `IndexError` means an index is out of range, and `ZeroDivisionError` means division by zero. These category names borrow from Python's habits, so that people familiar with Python can quickly understand what they mean.

Syntax errors use the `SyntaxError` prefix. They are produced during parsing, so the script is rejected before it runs. Runtime errors are produced during evaluation, and they only appear when execution reaches the statement that fails. This distinction tells developers at which stage an error occurred, so they can choose different fixing strategies.

Error propagation relies on C++ exception handling. When the evaluator hits an error, it throws a `DcError`, and the exception travels up the call stack until the command-line program catches it. Scripts currently have no `try` statement, so during this propagation the error always reaches the top level. This is a current limitation of `dc`; the roadmap lists error handling as a feature still to be implemented.

After catching an error, the command-line program formats the output uniformly. `dc run` prints the file name, the line number and the message, then prints the segfault message and exits with code 4. `dc eval` prints only the line number and the message, with no segfault message. The difference reflects the two commands' roles: `run` is part of a complete dream, while `eval` is a tool for daytime exploration.

`assert` and `raise` are two functions through which a script can produce errors on purpose. `assert` throws `AssertionError` when its condition is false, and the caller can specify the message. `raise` throws `Error` directly, and it is used to mark a situation that should never happen. These two functions let a script express its own expectations, instead of relying on implicit checks by the runtime.

The accuracy of line numbers is also worth noting. Every syntax node records the line it is on, and the evaluator uses the current node's line when it throws an error. As a result, an error usually points to the line that caused the problem, not to the beginning or end of a function. This is especially helpful when debugging long functions.

In short, the error model of `dc` aims for clarity and consistency. With one exception type, one message format and one output method, error reports are predictable, and interpreting them after waking has a clear starting point.

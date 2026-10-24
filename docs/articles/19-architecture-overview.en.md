# 19 Architecture Overview: From Source Code to the Subconscious

[中文](./19-architecture-overview.md) | [English](./19-architecture-overview.en.md)

The implementation of `dc` can be divided into several clear layers. A `.dc` file goes in at one end, and by the time the result comes out, it has passed through lexical analysis, syntax analysis, a syntax tree, an evaluator, and runtime values, in that order. This article gives an overview of the whole pipeline. The articles that follow will take each stage in turn.

The starting point is the source file. `dc run` reads the contents of the file and hands them to the interpreter as a string. The interpreter first calls the lexer, which cuts the string into tokens. A token is the smallest meaningful unit: a keyword, an identifier, a number, a string, or an operator. The lexer does not understand structure. Its only job is to recognize what each fragment is.

Next, the parser reads the token sequence and builds a syntax tree according to the grammar rules. The syntax tree is a tree made of nodes, and each node stands for a statement or an expression. For example, a function call produces a `Call` node, whose children are the function being called and its arguments. The parser is a recursive-descent parser: each kind of grammar construct has its own function.

Once the syntax tree exists, the evaluator gets to work. It starts at the root, walks each node recursively, and carries out the appropriate action based on the node's type. When it meets a `Let` node, for example, it defines a variable in the current scope. When it meets a `Binary` node, it first computes the values on the left and the right, and then applies the operator. The evaluator maintains an environment chain, which it uses to look up variables.

The data the evaluator works on are runtime values. These can be nil, booleans, numbers, strings, lists, dictionaries, or functions. All of them are represented by a single `Value` type, which stores the different kinds internally using the standard library's variant type. Lists, dictionaries, and functions have reference semantics, which means several variables can point to the same object.

The results of a run are shown on standard output. A `print` in a script writes directly to the output stream. Errors are caught, tagged with line-number information, and finally the command-line program decides how to display them and what exit code to return.

Each layer of this architecture tries to stay independent. The lexer knows nothing about grammar, the parser knows nothing about the runtime, and the evaluator does not care about the command line. This separation lets each part be tested on its own, and it also makes extension possible: to add a new built-in function, you only need to change the standard library, not the parsing logic.

It is worth mentioning that the whole implementation is a tree-walking interpreter. It does not generate machine code, and it does no optimization. This choice gives up some speed in exchange for simplicity and readability. For a technique like dream coding, a clear structure matters more than execution efficiency, because the developer needs to understand how the dream was worked out.

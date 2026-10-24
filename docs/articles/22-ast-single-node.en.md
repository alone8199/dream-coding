# 22 The Single-Node Syntax Tree

[中文](./22-ast-single-node.md) | [English](./22-ast-single-node.en.md)

The syntax tree is the product of parsing and the input to the evaluator. In many interpreters, each kind of syntactic construct has its own class: addition nodes, function-call nodes, and conditional nodes each get a class. `dc` takes a different approach. All nodes share one structure, and a kind field determines what each node means.

The structure is very simple. It contains a kind field, which says whether the node is a number, a string, an identifier, a function call, a conditional, or some other type. It contains a line number, used for error reporting. It contains a text field, which holds an identifier's name, an operator, or the contents of a string. It contains a number field, which holds a numeric literal or a boolean. It contains a list of names, which holds the parameter names of a function. And it contains a list of child nodes, which hold subexpressions or substatements.

The benefit of this design is simplicity. The whole syntax tree needs only one type, so the code for traversing and processing it can be concentrated in one place. When a new syntactic construct is added, you only add a value to the kind enumeration, then add one branch in the parser and one in the evaluator. There is no need to define a new class or to deal with complicated inheritance.

The cost is weaker type safety. The compiler cannot check whether a node has the right number of children, and it cannot guarantee that the text field has a value for certain kinds. `dc` makes up for this with conventions. For example, a binary-operation node always has exactly two children, and a function-call node's first child is the object being called while the remaining children are its arguments. These conventions are written in comments and are also reflected in the parser's implementation.

Each kind of node has a clear meaning. A block node represents a group of statements, and its children are executed in order. The children of a conditional node are, in sequence, the condition, the branch, and an optional else branch. The children of a loop node are the condition and the body. A function-declaration node stores the function's name, its parameter names, and its body. These conventions let the evaluator take out exactly the fields it needs, directly based on the kind.

Nodes are managed with shared pointers. The reason is that a function value must hold a reference to its body, and the body may outlive the statement that defined it. A shared pointer guarantees that even if some part of the syntax tree is captured by a function, it will not be freed early.

From a maintenance point of view, a single-node structure requires the developer to build good commenting habits. The meaning of each kind's fields should be documented next to the enumeration, so that later readers can understand it quickly. The `ast.hpp` of `dc` does exactly this: it notes the meaning of the children right beside each kind.

In short, the single-node structure is a design that trades simplicity for flexibility. It is not the only correct choice, but for an interpreter of the size of `dc`, it keeps the total amount of code within what one person can fully understand.

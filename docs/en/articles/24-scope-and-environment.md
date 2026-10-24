# 24 Scope and the Environment Chain

Scope determines where in a program a name can be accessed. `dc` implements scope through an environment chain. Each level of environment holds the names defined at that level and points to the enclosing environment. To look up a name, the evaluator starts in the current environment and searches outward, level by level, until it finds the name.

The structure of an environment is simple. It contains a mapping from names to values, and a pointer to its parent environment. The global environment has no parent; it is the end of the chain. In `dc`, the built-in functions are defined in a root environment, and the top-level environment of user code uses it as its parent. This means user code can call the built-in functions directly. However, this does not protect them from being changed: an assignment without `let` searches up the whole chain, so it will overwrite a built-in's binding if the name matches.

Defining a variable and modifying a variable are two different operations. Definition uses the `let` keyword, which always creates a new binding in the current environment. Even if a variable of the same name already exists in an outer level, `let` creates a new variable at the current level that shadows it. Modification uses an ordinary assignment, which searches along the environment chain and updates the first variable it finds with that name. If it can never find one, it reports a `NameError`.

This distinction helps prevent accidental changes. The developer must explicitly use `let` to create a variable; otherwise the assignment fails. This makes typos surface quickly. If you write `cuont` instead of `count`, the program reports an error immediately instead of quietly creating a new variable.

Blocks create new environments. The branches or bodies of `if`, `while`, and `for`, as well as any block enclosed in braces, create a child environment when they execute. A variable defined with `let` inside a block is visible only within that block, and it disappears when the block ends. This keeps temporary variables from polluting the outer scope.

Function calls also create new environments. The function's parameters and its local variables are stored at this level. The parent of this level is not the environment of the caller, but the environment where the function was defined. This is the foundation of closures, which we will discuss in the next article.

Each iteration of a loop creates a new environment. The loop variable is a fresh binding on every iteration, so a closure created inside the loop body captures its own independent value, rather than all of them pointing to the same variable. This detail matters a great deal for using closures correctly.

Module imports have their own environment too. When you import a user module, its code runs in a separate environment whose parent is the root environment. After execution finishes, the evaluator collects every top-level name in the module environment that does not begin with an underscore into a dictionary, and returns that as the module object. This way, private names inside a module do not leak to the outside.

Lookup in the environment chain is linear, with each level checked in turn. For the small number of levels found in dream scripts, this cost is negligible. If optimization were ever needed, one could consider resolving variables to fixed slots, but that would add implementation complexity, which is not worth it at the current scale.

In short, the environment chain is a simple and clear model of scope. Its rules can be explained in a few sentences, and it is enough to support closures, modules, and block-level scope.

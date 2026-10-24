# 29 The Import Mechanism and Module Cache

Modules let dream scripts reuse code. `dc` loads modules with the `import` statement, and its lookup rules, caching and cycle checks follow clear conventions. Understanding these conventions helps developers organize dependencies among multiple scripts.

`import` has two forms. The first writes a bare name, for example `import math;`, which loads a standard library module or a registered module. The second writes a string path, for example `import "lib/greet.dc";`, which loads a file. The two forms search in different orders, but both bind the module to a name in the current scope.

For the name form, the evaluator first checks the module cache. If the module has already been loaded, it returns the cached result directly. If there is no cache entry, it looks up the registered module factory. The standard library's `math` and `dream` are provided this way. Registered modules take priority over file modules, which means a `.dc` file with the same name will not override a built-in module.

For the path form, the evaluator joins the path with the directory of the current file to get the full file location. If the path does not end in `.dc`, the suffix is added automatically. If the file exists, the evaluator reads and parses its contents, then runs it in a separate environment.

The name a module is bound to is taken from the file name, with the directory and suffix removed. For example, `import "lib/greet.dc";` binds the module to the name `greet`. This means that if two files in different directories share a name, they will produce the same binding name within their respective scopes, so the import locations must be used to tell them apart.

All top-level names in a module that do not begin with an underscore become members of the module object. Names beginning with an underscore are private: they can be used inside the module but are not exported. This convention matches Python's habits, and it lets the public interface of a module be expressed through naming, without any extra declarations.

A module runs only once. On the first import, the evaluator executes the module's code and caches the result. Later imports of the same module use the cached object directly and do not run it again. This caching ensures that side effects in a module happen only once and avoids repeated overhead.

Circular imports are a situation to guard against. If module A imports module B, and module B imports module A, an endless dependency chain forms. `dc` detects this with an import stack. When a module is requested while it is still being loaded, it reports `ImportError: circular import` and names the module.

When a module cannot be found, the import reports `ImportError: no module named` and gives the module's name. This error usually means the path is wrong or the file is not in the expected directory. Because file paths are relative to the current file, moving a file may require updating its relative paths accordingly.

In short, the import mechanism uses a simple set of rules to support module reuse, caching and isolation. Its design goal is predictability: developers should be able to infer the result of an import from the rules alone.

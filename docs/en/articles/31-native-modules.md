# 31 Registering C++ Modules

If a group of native functions needs to be used together, registering each one separately in the global environment becomes messy. `dc` offers another approach: organize them into a module. Modules are loaded in scripts with `import`, and their functions are accessed with dot notation, just like the `math` module in the standard library.

Modules are registered with the interpreter's `register_module` method. It takes two arguments: the module's name, and a factory function. The factory function is called the first time the module is imported, and its return value should be a dictionary; each entry in the dictionary becomes a member of the module.

The following example registers a module named `clock`, which contains a version number and a function that gets the current time:

```cpp
interp.register_module("clock", [](dc::Interpreter&) {
    dc::Value m = dc::Value::dict();
    auto& items = std::get<dc::DictPtr>(m.data)->items;

    items["version"] = dc::Value::str("1.0");
    items["now"] = dc::make_native("now", [](dc::Interpreter&, std::vector<dc::Value>&, int) {
        return dc::Value::num(static_cast<double>(std::time(nullptr)));
    });
    return m;
});
```

In a script, it can be used like this:

```dc
import clock;
print(clock.version, clock.now() > 0);
```

The factory's result is cached. The factory runs on the first import, and later imports of the same module return the cached object directly. This ensures that the module is initialized only once and that its state stays consistent across multiple imports.

Module members can be any value, including numbers, strings, lists, dictionaries and functions. Plain values suit constants, such as a version number; functions suit a module's behavior, such as getting the time. A module can even contain other modules, as long as they are placed inside a dictionary.

Registered modules work the same way as modules written in `.dc`. A script does not need to know whether a module is implemented in C++ or in a dream script; it only accesses it by name and fields. This consistency makes extension transparent, and it lets teams choose the implementation that suits them.

Note that when a registered module has the same name as a standard library module, it overrides the standard library version. The reason is that both share the same registry, and whichever is registered later replaces the earlier one. File modules are searched last, and a `.dc` file is read only if the registry does not contain the name. Therefore, avoid names such as `math` and `dream` for your modules unless you really intend to replace them.

When designing a module, it is recommended to keep related functionality in one module and to document the argument and return value conventions for each function. A module is the boundary between a script and C++ code, and a clear boundary makes it easier for maintainers on both sides to understand each other's intent.

In short, registering modules is the formal way to make C++ functionality available to scripts. Its core convention is a single rule: the factory returns a dictionary. Follow it, and any C++ code can become a module that scripts can import.

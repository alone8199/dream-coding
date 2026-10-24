# 30 Registering Native Functions

Native functions are functions implemented in C++ that dream scripts can call directly. They are the most direct form of `dc`'s extension capability: developers do not need to modify the interpreter itself, only register a function in C++ code to make it available to scripts.

Native functions are registered with the interpreter's `define_function` method. It takes two arguments: the function's name, and a function object that matches a specific signature. The signature has three parts: a reference to the interpreter, the list of arguments, and the line of the call. The return value is of type `Value`, representing the result seen by the script.

Here is a simple example. It registers a function named `shout`, which appends three exclamation marks to the string it receives:

```cpp
interp.define_function("shout", [](dc::Interpreter& in, std::vector<dc::Value>& args, int line) {
    if (args.size() != 1 || !args[0].is_str()) {
        throw dc::DcError("TypeError: shout() expects one string", line);
    }
    return dc::Value::str(std::get<std::string>(args[0].data) + "!!!");
});
```

The function first checks the number and type of its arguments. If they do not meet the requirements, it throws a `DcError` and attaches the line number it was passed. This matters: an error thrown by a native function should point to the line in the script that called it, not to some line in the C++ code. That way, the script's developer can find the problem in their own code.

Checking arguments is a basic responsibility of a native function. The `dc` interpreter does not type-check arguments for native functions on their behalf, because it cannot know what arguments each function expects. Therefore, every native function must verify the number and types of its arguments itself, and give a clear error message when they do not meet the requirements.

Native functions can use the interpreter's output stream, for example `in.out()`. If a function needs to print information, it should do so through this stream rather than writing directly to standard output. The benefit is that the output destination can be managed in one place, and it is easier to capture in tests.

Native functions can also call functions defined in the script. The interpreter provides a `call` method, which takes a function value, an argument list and a line number, then performs the call and returns the result. This lets native functions cooperate with scripts, for example to implement a sorting function that accepts a callback.

Registered functions are placed in the global environment, so every script can use them directly. If a name conflicts with a built-in function, the registered function overrides the built-in one. Therefore, before registering a new function, make sure its name will not cause confusion with existing functionality.

In short, native functions give `dc` its simplest form of extension. They do not require understanding the interpreter's internal structure. By following the conventions for arguments and errors, a script can gain new capabilities.

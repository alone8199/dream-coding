# Extending dc

[中文](./extending.md) | [English](./extending.en.md)

There are three ways to extend dc, from simplest to most involved:

1. **Write a `.dc` module**: no C++ needed. Load it with `import "xxx.dc";`.
2. **Register a native function in C++**: `Interpreter::define_function`. Scripts call it directly.
3. **Register a native module in C++**: `Interpreter::register_module`. Scripts load it with `import name;`, just like the standard `math` and `dream` modules.

## 1. Write a `.dc` module

```dc
# lib/greet.dc
let _prefix = "dream";          # leading underscore: private, not exported

fn greet(name) {
    return "[" + _prefix + "] hello, " + name;
}
```

```dc
# main.dc
import "lib/greet.dc";
print(greet.greet("sleeper"));
```

## 2. Register a native function

```cpp
#include "dc/interpreter.hpp"

std::ostringstream out;
dc::Interpreter interp(out);

interp.define_function("shout", [](dc::Interpreter& in, std::vector<dc::Value>& args, int line) {
    if (args.size() != 1 || !args[0].is_str()) {
        throw dc::DcError("TypeError: shout() expects one string", line);
    }
    return dc::Value::str(std::get<std::string>(args[0].data) + "!!!");
});

interp.run("print(shout(\"wake up\"));");   // wake up!!!
```

- `in`: the interpreter. Use `in.out()` to write output.
- `args`: the already-evaluated arguments.
- `line`: the calling line. Pass it to `DcError` so errors point at the right place in the script.

## 3. Register a native module

A module is a dict. The factory runs the first time the module is imported, and the result is cached.

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

In a script:

```dc
import clock;
print(clock.version, clock.now() > 0);
```

## Common value operations

| Operation | Code |
|---|---|
| Create | `Value::num(1)`, `Value::str("x")`, `Value::boolean(true)`, `Value::nil()`, `Value::list({...})`, `Value::dict()` |
| Type checks | `v.is_num()`, `v.is_str()`, `v.is_list()`, `v.is_dict()`, `v.is_func()` |
| Extract | `std::get<double>(v.data)`, `std::get<std::string>(v.data)`, `std::get<ListPtr>(v.data)->items` |
| To text | `to_display(v)` (what print uses), `to_repr(v)` (strings in quotes) |
| Equality | `values_equal(a, b)` |
| Call a dc function | `interp.call(fn, args, line)` |

## Notes

- Lists, dicts, and functions have reference semantics: changes are visible through every reference.
- Module dict keys are strings. Numeric keys are converted to strings.
- Native functions should use the `line` they receive when throwing `DcError`, so messages point at the right script line.
- There is no dynamic plugin loading (`.so` / `.dll`) yet. New features have to be compiled in and registered with `register_module`.

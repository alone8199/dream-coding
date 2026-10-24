# 扩展 dc

[中文](./extending.md) | [English](./extending.en.md)

dc 有三种扩展方式，从简单到复杂：

1. **写 `.dc` 模块**：不需要 C++，用 `import "xxx.dc";` 加载。
2. **在 C++ 里注册函数**：`Interpreter::define_function`，脚本里直接调用。
3. **在 C++ 里注册模块**：`Interpreter::register_module`，脚本里用 `import name;` 加载，和标准库的 `math`、`dream` 用法一致。

## 1. 写 `.dc` 模块

```dc
# lib/greet.dc
let _prefix = "dream";          # 下划线开头：私有，不导出

fn greet(name) {
    return "[" + _prefix + "] hello, " + name;
}
```

```dc
# main.dc
import "lib/greet.dc";
print(greet.greet("sleeper"));
```

## 2. 注册原生函数

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

- `in`：解释器，可以用 `in.out()` 写输出。
- `args`：参数，已经求值。
- `line`：调用所在的行号，抛错时传给 `DcError`。

## 3. 注册模块

模块就是一个字典。工厂函数在第一次 `import` 时调用，结果会被缓存。

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

脚本里：

```dc
import clock;
print(clock.version, clock.now() > 0);
```

## 值的常用操作

| 操作 | 代码 |
|---|---|
| 创建 | `Value::num(1)`、`Value::str("x")`、`Value::boolean(true)`、`Value::nil()`、`Value::list({...})`、`Value::dict()` |
| 判断类型 | `v.is_num()`、`v.is_str()`、`v.is_list()`、`v.is_dict()`、`v.is_func()` |
| 取出 C++ 值 | `std::get<double>(v.data)`、`std::get<std::string>(v.data)`、`std::get<ListPtr>(v.data)->items` |
| 转成文字 | `to_display(v)`（和 print 一样）、`to_repr(v)`（字符串带引号） |
| 相等 | `values_equal(a, b)` |
| 调用 dc 函数 | `interp.call(fn, args, line)` |

## 注意事项

- 列表、字典、函数是引用语义：修改会影响所有引用。
- 模块的字典键是字符串，数字键会被转成字符串。
- 原生函数抛出的 `DcError` 应该使用调用处传入的 `line`，这样错误信息会指向脚本中正确的行。
- 目前没有动态加载 `.so` / `.dll` 插件的机制。要加新功能，需要在 C++ 里链接进来，然后调用 `register_module`。

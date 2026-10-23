# Dream Coding

> 编译于潜意识，运行于不存在。
> 在梦里写代码，醒来只剩 bug 和诗。

**Dream Coding** 是一门在睡眠中完成开发的编程语言。源文件后缀为 `.dc`，语法接近 Python 和 C，解释器是用 C++17 写的。程序在梦中已经完美执行过，醒来后你得到的只是它的尸体。

```dc
import dream;

fn fib(n) {
    if n < 2 { return n; }
    return fib(n - 1) + fib(n - 2);
}

print("fib(10) =", fib(10));
dream.wake();
```

```
$ dc run fib.dc
fib(10) = 55
Segmentation fault (dream dumped)
```

---

## 快速开始

### 构建

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

产物是 `build/dc`（Windows 上是 `build/Release/dc.exe`）。

### 命令

| 命令 | 作用 |
|---|---|
| `dc run <file.dc> [args...]` | 运行脚本，额外参数可通过 `argv` 读取 |
| `dc check <file.dc>` | 只做语法检查 |
| `dc eval "<code>"` | 运行一行代码 |
| `dc sleep [hours]` | 进入 REM 周期，默认 8 小时，不足 7 小时会超时 |
| `dc wake` | 醒来，输出 segfault |
| `dc version` / `dc help` | 版本与帮助 |

运行时错误会带上文件名和行号，并以退出码 4 结束：

```
$ dc run bad.dc
bad.dc:2: NameError: name 'zzz' is not defined
Segmentation fault (dream dumped)
```

## 语言概览

```dc
# 变量与函数
let name = "dream";
fn greet(who) { return "hello, " + who; }

# 列表、字典、闭包
let xs = [1, 2, 3];
xs.push(4);
let config = {debug: true, level: 3};

fn counter() {
    let n = 0;
    return fn() { n = n + 1; return n; };
}

# 控制流
for x in xs {
    if x % 2 == 0 { print(x, "is even"); }
}

# 模块
import math;
print(math.sqrt(16), greet(name));
```

完整语法见 [docs/language.md](./docs/language.md)。

## 扩展

dc 可以用三种方式扩展：写 `.dc` 模块、在 C++ 中注册函数、在 C++ 中注册模块。例如：

```cpp
interp.register_module("clock", [](dc::Interpreter&) { /* 返回一个字典 */ });
```

脚本里 `import clock;` 之后就能使用。详见 [docs/extending.md](./docs/extending.md)。

## 项目结构

```
dream-coding/
├── src/
│   ├── main.cpp            # dc 程序入口
│   ├── main.dc             # 一个能运行的梦
│   └── dc/
│       ├── lexer.{hpp,cpp}     # 词法分析
│       ├── parser.{hpp,cpp}    # 递归下降语法分析
│       ├── ast.hpp             # 语法树
│       ├── value.{hpp,cpp}     # 运行时的值
│       ├── interpreter.{hpp,cpp}  # 求值器、作用域、函数、导入
│       ├── builtins.cpp        # 全局函数与容器方法
│       ├── stdlib.cpp          # math、dream 模块
│       ├── runner.{hpp,cpp}    # 命令行
│       └── error.hpp           # DcError
├── examples/               # 示例脚本（CI 会运行它们）
├── tests/                  # C++ 单元测试与解释器测试
├── docs/
│   ├── language.md         # 语言参考
│   ├── extending.md        # 扩展指南
│   └── vibe-vs-dream.md    # 与 vibe coding 的对比
├── pseudocode/             # 早期的伪代码草稿（已不能运行）
├── .github/workflows/build.yml
├── CMakeLists.txt
├── CHANGELOG.md
├── CONTRIBUTING.md
└── LICENSE
```

## 持续集成

`.github/workflows/build.yml` 会在 Ubuntu、macOS、Windows 上构建并运行全部测试，然后检查示例的语法和输出。手动运行 workflow 并填写 `version`（例如 `v0.3.0`）会创建 Release；推送 `v*` 标签也会触发发布。

## 常见问题

**Q：程序跑不通怎么办？**
A：它在梦里跑通过了。请确认你的醒来时机是否正确。

**Q：为什么报 `RecursionError: stack overflow in subconscious`？**
A：递归太深了，上限是 200 层。建议在入睡前少想一点"我要写递归"。

**Q：能用 Python 调用 dc 吗？**
A：目前没有绑定。dc 本身就是 C++ 库，可以直接嵌入 C++ 程序。

**Q：这是一个真正的编程语言吗？**
A：是，但请不要把它部署到生产环境，除非你的生产环境也在梦里。

## 路线图

- [x] 词法分析、语法分析、树遍历解释器
- [x] 函数、闭包、递归、列表、字典、字符串方法
- [x] 模块导入（`.dc` 文件与 C++ 注册）
- [x] 命令行：`run`、`check`、`eval`
- [ ] 错误处理（`try` / `catch`）
- [ ] 类与对象
- [ ] 动态加载 `.so` / `.dll` 插件
- [ ] 解决 "醒来后忘记梦的内容" 问题（目前无解）

## 贡献

请先阅读 [CONTRIBUTING.md](./CONTRIBUTING.md)。注意：提交的代码最好是在梦里写的。

## 许可证

见 [LICENSE](./LICENSE)。

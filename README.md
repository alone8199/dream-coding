# Dream Coding

> 编译于潜意识，运行于不存在。
> 在梦里写代码，醒来只剩 bug 和诗。

**Dream Coding** 是一种在睡眠中完成开发的技术。你不敲键盘，只是在睡前把想法写下来，让潜意识去编译和运行它；醒来后看到的是一个跑不起来的乌托邦，以及一行终端输出：

```
Segmentation fault (dream dumped)
```

这就是正常的结果。

---

## 流程

Dream Coding 的一次完整循环：

1. **白天**：打开 `src/main.dc`，读一遍，然后关掉它。
2. **入睡**：`dc sleep`，进入 REM 周期，不少于 7 小时。
3. **醒来**：`dc wake`，看到 segfault，不要 debug。
4. **解梦**：如果还记得梦里的内容，写下来；不记得也没关系。
5. **提交**：只在入睡时提交，醒来后假装什么都没发生。

vibe coding 的尽头是 Stack Overflow，dream coding 的尽头是弗洛伊德。完整对比见 [docs/vibe-vs-dream.md](./docs/vibe-vs-dream.md)。

## 工具：dc

`dc` 是 Dream Coding 的配套工具，用 C++17 写成，负责把梦境脚本（`.dc` 文件）交给潜意识执行。它不是 Dream Coding 本身，只是这套仪式的一个执行器。

```
$ dc run examples/fib.dc
fib(0..9): 0, 1, 1, 2, 3, 5, 8, 13, 21, 34
counter after 3 calls: 3
```

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
| `dc sleep [hours]` | 进入 REM 周期，默认 8 小时，不足 7 小时会超时 |
| `dc wake` | 醒来，输出 segfault |
| `dc run <file.dc> [args...]` | 在潜意识中编译，在不存在中运行，额外参数通过 `argv` 读取 |
| `dc check <file.dc>` | 只做语法检查，不运行 |
| `dc eval "<code>"` | 运行一行代码 |
| `dc version` / `dc help` | 版本与帮助 |

脚本出错时，会带上文件名和行号，并输出 segfault，以退出码 4 结束：

```
$ dc run bad.dc
bad.dc:2: NameError: name 'zzz' is not defined
Segmentation fault (dream dumped)
```

## 梦境脚本

`.dc` 脚本的语法接近 Python 和 C，示例：

```dc
import dream;

fn fib(n) {
    if n < 2 { return n; }
    return fib(n - 1) + fib(n - 2);
}

print("fib(10) =", fib(10));
dream.wake();
```

语法细节见 [docs/dc-reference.md](./docs/dc-reference.md)。如果想给 `dc` 增加新的函数或模块，见 [docs/extending.md](./docs/extending.md)。

## 项目结构

```
dream-coding/
├── src/
│   ├── main.cpp            # dc 入口
│   ├── main.dc             # 第一个梦
│   └── dc/                 # dc 的实现
│       ├── lexer, parser, ast        # 读懂梦境脚本
│       ├── value, interpreter        # 在潜意识里执行
│       ├── builtins, stdlib          # 内置函数与 math、dream 模块
│       └── runner, error             # 命令行与错误
├── examples/               # 示例梦境（CI 会运行它们）
├── tests/                  # 单元测试与解释器测试
├── docs/
│   ├── dc-reference.md     # dc 脚本参考
│   ├── extending.md        # 扩展 dc
│   └── vibe-vs-dream.md    # 与 vibe coding 的对比
├── pseudocode/             # 早期的伪代码草稿，只用来做梦
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

**Q：dc 是一门编程语言吗？**
A：不是。dc 只是执行梦境脚本的工具。Dream Coding 的重点在于睡眠与醒来的流程，写哪种语言都可以，只是醒来以后一切都会变成 bug 和诗。

## 路线图

- [x] 睡眠、醒来、segfault 的仪式化流程
- [x] `dc` 执行器：脚本、模块、命令行
- [ ] 梦境日志：记录醒来后还记得的部分
- [ ] 动态加载插件
- [ ] 解决"醒来后忘记梦的内容"的问题（目前无解）

## 贡献

请先阅读 [CONTRIBUTING.md](./CONTRIBUTING.md)。注意：提交的代码最好是在梦里写的。

## 许可证

见 [LICENSE](./LICENSE)。

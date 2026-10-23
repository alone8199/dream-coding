# Dream Coding

[中文](./README.md) | [English](./README.en.md)

> 编译于潜意识，运行于不存在。
> 在梦里写代码，醒来只剩 bug 和诗。

**Dream Coding** 是一门在睡眠中完成开发的编程范式。源文件后缀为 `.dc`，编译器运行在你的 REM 周期里，程序在梦中已经完美执行过，醒来后你得到的只是它的尸体。

```
$ dc run examples/fib.dc
fib(0..9): 0, 1, 1, 2, 3, 5, 8, 13, 21, 34
counter after 3 calls: 3
```

---

## 目录

- [为什么要 Dream Coding](#为什么要-dream-coding)
- [与 vibe coding 的区别](#与-vibe-coding-的区别)
- [快速开始](#快速开始)
- [项目结构](#项目结构)
- [语法速览](#语法速览)
- [常见问题](#常见问题)
- [路线图](#路线图)
- [许可证](#许可证)

## 为什么要 Dream Coding

- **零键盘消耗**：不需要摇晃键盘，只需要闭眼。
- **无限运行时**：梦里没有内存上限，也没有 OOM。
- **完美测试覆盖率**：在梦里，每个测试都通过了。
- **天然异步**：你的代码会在你醒来之前完成所有回调。
- **无需 code review**：没有人能审查一个只存在于潜意识里的 PR。

## 与 vibe coding 的区别

| | vibe coding | dream coding |
|---|---|---|
| 你的动作 | 摇晃键盘，让 AI 猜你的心情 | 关掉 IDE，闭上眼睛 |
| 产出 | 能跑的屎山 | 跑不起来的乌托邦 |
| 调试方式 | 去 Stack Overflow | 去弗洛伊德那里解梦 |
| 提交方式 | `git commit -m "fix"` | 入睡 |
| 部署方式 | 上线，然后祈祷 | 醒来，假装什么都没发生 |
| 最终归宿 | Stack Overflow | 弗洛伊德 |

完整对比见 [docs/zh/vibe-vs-dream.md](./docs/zh/vibe-vs-dream.md)。

## 快速开始

### 1. 准备环境

你需要：

- 一张床
- 至少 7 小时的睡眠时间（不足则 `dc sleep` 会报 `TimeoutError: sleep too short`）
- 褪黑素一粒（可选，但强烈推荐）
- 一个 C++17 编译器和 CMake，用来构建 `dc`

构建：

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

产物是 `build/dc`（Windows 上是 `build/Release/dc.exe`）。

### 2. 写代码

在白天打开 `src/main.dc`，阅读一遍，然后关掉它。

### 3. 入睡

```
$ dc sleep
Entering REM cycle...
zzz... (8h)
```

### 4. 醒来

```
$ dc wake
Waking up...
Segmentation fault (dream dumped)
```

这就是正常的输出。请不要 debug，请去解梦。

更多命令（`run`、`check`、`eval`）和脚本的运行方式，见 [docs/zh/usage.md](./docs/zh/usage.md)。

## 项目结构

```
dream-coding/
├── src/
│   ├── main.cpp            # dc 程序入口
│   ├── main.dc             # 程序入口梦境，梦的开始
│   └── dc/                 # dc 的实现：词法、语法、求值、标准库、命令行
├── examples/               # 示例梦境（CI 会运行它们）
├── tests/                  # 单元测试与解释器测试
├── docs/
│   ├── zh/                 # 中文文档：使用说明、脚本参考、扩展、对比
│   └── en/                 # English docs
├── pseudocode/             # 早期的伪代码草稿，只用来做梦
├── README.md / README.en.md
├── CHANGELOG.md / CHANGELOG.en.md
├── CONTRIBUTING.md / CONTRIBUTING.en.md
└── LICENSE
```

## 语法速览

`.dc` 文件的语法接近 Python 和 C，用花括号分块，分号可以省略：

```dc
import dream;

fn fib(n) {
    if n < 2 { return n; }
    return fib(n - 1) + fib(n - 2);
}

print("fib(10) =", fib(10));
dream.wake();
```

输出（醒来后看到的）：

```
fib(10) = 55
Segmentation fault (dream dumped)
```

完整语法见 [docs/zh/scripts.md](./docs/zh/scripts.md)。

## 常见问题

**Q：程序跑不通怎么办？**
A：它在梦里跑通过了。请确认你的醒来时机是否正确。

**Q：为什么报 `RecursionError: stack overflow in subconscious`？**
A：你梦里的递归太深了，上限是 200 层。建议在入睡前少想一点"我要写递归"。

**Q：能用 Python 调用 `.dc` 吗？**
A：目前没有 Python 绑定。dc 是一个 C++ 库，可以直接嵌入 C++ 程序，见 [docs/zh/extending.md](./docs/zh/extending.md)。

**Q：这是一个真正的编程语言吗？**
A：这是一个梗。请不要把它部署到生产环境，除非你的生产环境也在梦里。

**Q：有 linter 吗？**
A：有。它在你的潜意识里，叫做"第二天早上的羞耻感"。

## 路线图

- [x] 确定后缀名：`.dc`
- [x] 写下第一段伪代码，并把它变成能运行的脚本
- [x] 实现 `dc sleep` 命令
- [x] 实现 `dc wake` 命令（输出 segfault）
- [x] 模块导入与标准库（`math`、`dream`）
- [ ] 支持梦中的异步 I/O
- [ ] 解决 "醒来后忘记梦的内容" 问题（目前无解）

## Star History
[![stars](https://afterglow.watch/badge/OWNER/REPO)](https://afterglow.watch)

## 贡献

欢迎贡献。请先阅读 [CONTRIBUTING.md](./CONTRIBUTING.md)。注意：提交的代码最好是在梦里写的。

## 许可证

见 [LICENSE](./LICENSE)。

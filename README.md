# Dream Coding

[中文](./README.md) | [English](./README.en.md)

> 编译于潜意识，运行于不存在。
> 在梦里写代码，醒来只剩 bug 和诗。

**Dream Coding** 是一种在睡眠中完成开发的技术。你在白天写下梦境，在夜里让它运行，醒来后得到的只是一行段错误和一堆需要解释的感受。

```
$ dc run dreams/fib.dc
fib(0..9): 0, 1, 1, 2, 3, 5, 8, 13, 21, 34
counter after 3 calls: 3
```

---

## 目录

- [快速开始](#快速开始)
- [关于这项技术](#关于这项技术)
- [阅读](#阅读)
- [梦境脚本记法](#梦境脚本记法)
- [项目结构](#项目结构)
- [常见问题](#常见问题)
- [路线图](#路线图)
- [贡献](#贡献)
- [许可证](#许可证)

## 快速开始

### 1. 准备

- 一张床
- 至少 7 小时的睡眠（不足则 `dc sleep` 会报 `TimeoutError: sleep too short`）
- 一个 C++17 编译器和 CMake，用来构建 `dc`

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

产物是 `build/dc`（Windows 上是 `build/Release/dc.exe`）。

### 2. 白天写梦

打开 `dreams/main.dc`，读一遍，然后关掉它。

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

这是正常输出。请不要 debug，请去解梦。

其他命令（`run`、`check`、`eval`）的用法见 [docs/zh/usage.md](./docs/zh/usage.md)。

## 关于这项技术

Dream Coding 的核心是三件事：白天写下意图，夜里让它执行，醒来后解读结果。它不是一门编程语言，而是一套工作方式。`dc` 只是让这套方式能够运行的工具。

| | vibe coding | dream coding |
|---|---|---|
| 你的动作 | 摇晃键盘，让 AI 猜你的心情 | 关掉 IDE，闭上眼睛 |
| 产出 | 能跑的屎山 | 跑不起来的乌托邦 |
| 调试方式 | 去 Stack Overflow | 去弗洛伊德那里解梦 |
| 提交方式 | `git commit -m "fix"` | 入睡 |
| 部署方式 | 上线，然后祈祷 | 醒来，假装什么都没发生 |

完整对比见 [docs/zh/vibe-vs-dream.md](./docs/zh/vibe-vs-dream.md)。

## 阅读

项目的文字分为三类，按需选择：

- **技术文章**：36 篇，从梦境编程的原则讲到 `dc` 的词法分析、语法树、值表示和模块系统。入口见 [docs/zh/articles/](./docs/zh/articles/README.md)。
- **脚本与扩展参考**：梦境脚本的完整记法，以及如何用 C++ 注册原生函数和模块。见 [docs/zh/scripts.md](./docs/zh/scripts.md) 和 [docs/zh/extending.md](./docs/zh/extending.md)。
- **杂谈**：诗、自白、对话、寓言和一本词典。与技术无关，或者说，只是绕着技术转了一圈。入口见 [docs/zh/essays/](./docs/zh/essays/README.md)。

## 梦境脚本记法

`.dc` 梦境脚本的记法接近 Python 和 C：代码块用花括号，分号可以省略。它是把想法交给潜意识的格式，不是需要别人维护的语言规范。

```dc
import dream;

fn fib(n) {
    if n < 2 { return n; }
    return fib(n - 1) + fib(n - 2);
}

print("fib(10) =", fib(10));
dream.wake();
```

醒来后看到的输出：

```
fib(10) = 55
Segmentation fault (dream dumped)
```

完整记法见 [docs/zh/scripts.md](./docs/zh/scripts.md)。

## 项目结构

```
dream-coding/
├── dreams/                 # 梦境：入口梦 main.dc 与示例梦（CI 会运行它们）
├── runtime/                # dc：让梦境在潜意识中执行的工具
│   ├── src/                # main.cpp 与 dc/ 下的实现
│   └── tests/              # 单元测试与解释器测试
├── docs/
│   ├── zh/                 # 中文：使用说明、脚本参考、扩展、对比
│   │   ├── articles/       # 技术文章（36 篇）
│   │   └── essays/         # 杂谈
│   └── en/                 # English: usage, script reference, extending, comparison
├── pseudocode/             # 早期的伪代码草稿，只用来做梦
├── README.md / README.en.md
├── CHANGELOG.md / CHANGELOG.en.md
├── CONTRIBUTING.md / CONTRIBUTING.en.md
└── LICENSE
```

## 常见问题

**Q：程序跑不通怎么办？**
A：它在梦里跑通过了。请确认你的醒来时机是否正确。

**Q：为什么报 `RecursionError: stack overflow in subconscious`？**
A：你梦里的递归太深了，上限是 200 层。建议入睡前少想一点"我要写递归"。

**Q：能用 Python 调用 `.dc` 吗？**
A：目前没有 Python 绑定。dc 是一个 C++ 库，可以直接嵌入 C++ 程序，见 [docs/zh/extending.md](./docs/zh/extending.md)。

**Q：这是一门真正的编程语言吗？**
A：这是一个梗。请不要把它部署到生产环境，除非你的生产环境也在梦里。

**Q：有 linter 吗？**
A：有。它在你的潜意识里，叫做"第二天早上的羞耻感"。

## 路线图

- [x] 确定后缀名：`.dc`
- [x] 写下第一段伪代码，并把它变成能运行的脚本
- [x] 实现 `dc sleep` 命令
- [x] 实现 `dc wake` 命令（输出段错误）
- [x] 模块导入与标准库（`math`、`dream`）
- [ ] 支持梦中的异步 I/O
- [ ] 解决"醒来后忘记梦的内容"问题（目前无解）

## Star History

<a href="https://www.star-history.com/?repos=alone8199%2Fdream-coding&type=date&legend=top-left">
 <picture>
   <source media="(prefers-color-scheme: dark)" srcset="https://api.star-history.com/chart?repos=alone8199/dream-coding&type=date&theme=dark&legend=top-left" />
   <source media="(prefers-color-scheme: light)" srcset="https://api.star-history.com/chart?repos=alone8199/dream-coding&type=date&legend=top-left" />
   <img alt="Star History Chart" src="https://api.star-history.com/chart?repos=alone8199/dream-coding&type=date&legend=top-left" />
 </picture>
</a>

## 贡献

欢迎贡献。请先阅读 [CONTRIBUTING.md](./CONTRIBUTING.md)。注意：提交的代码最好是在梦里写的。

## 许可证

见 [LICENSE](./LICENSE)。

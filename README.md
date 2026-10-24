# Dream Coding

[中文](./README.md) | [English](./README.en.md)

> 编译于潜意识，运行于不存在。
> 在梦里写代码，醒来只剩 bug 和诗。

**Dream Coding** 是一种在睡眠中完成开发的技术。白天写下梦境，夜里让它运行，醒来后得到的只是一行段错误，以及一堆需要解释的感受。`dc` 是让这套流程可以真正运行的命令行工具。

```
$ dc run dreams/fib.dc
fib(0..9): 0, 1, 1, 2, 3, 5, 8, 13, 21, 34
counter after 3 calls: 3
```

## 一天的流程

1. **白天写梦。** 打开 `dreams/main.dc`，读一遍，然后关掉它。
2. **入睡。** 运行 `dc sleep`，它会记录入睡并检查时长（默认八小时，不足七小时会报错）。
3. **醒来。** 运行 `dc wake`，输出 `Segmentation fault (dream dumped)`。这是正常的输出，请不要 debug。
4. **解梦。** 回忆梦里的内容，对照脚本，判断它应该怎样改。

```
$ dc sleep
Entering REM cycle...
zzz... (8h)

$ dc wake
Waking up...
Segmentation fault (dream dumped)
```

## 快速开始

需要一个支持 C++17 的编译器和 CMake。

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

产物是 `build/dc`（Windows 上是 `build/Release/dc.exe`）。常用命令有 `run`、`check`、`eval`、`sleep`、`wake`，完整说明见 [docs/usage.md](./docs/usage.md)。

## 梦境脚本

`.dc` 脚本的记法接近 Python 和 C：代码块用花括号，分号可选。它是把想法交给潜意识的格式，不是需要别人维护的语言规范。

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

## 文档

| 想了解什么 | 去哪里 |
|---|---|
| 命令怎么用、退出码是什么 | [使用说明](./docs/usage.md) |
| 脚本的完整记法 | [脚本参考](./docs/scripts.md) |
| 如何用 C++ 扩展 `dc` | [扩展指南](./docs/extending.md) |
| 与 vibe coding 有什么不同 | [对比](./docs/vibe-vs-dream.md) |
| 解释器与工程实现的细节 | [技术文章](./docs/articles/README.md)（36 篇） |
| 不那么正式的文字 | [杂谈](./docs/essays/README.md)（诗、自白、对话、寓言、词典） |

## 项目结构

```
dream-coding/
├── dreams/          梦境：入口梦 main.dc 与示例（CI 会运行它们）
├── runtime/         dc 的源码与测试
├── docs/            文档：用法、记法、扩展、对比、技术文章、杂谈
├── pseudocode/      早期伪代码草稿，只用来做梦
├── README.md        本文件（中文）
├── README.en.md     English README
├── CHANGELOG.md     变更记录
├── CONTRIBUTING.md  贡献指南
└── LICENSE
```

## 常见问题

**程序跑不通怎么办？**
它在梦里跑通过了。请确认你醒来的时机是否正确。

**为什么报 `RecursionError: stack overflow in subconscious`？**
梦里的递归太深了，上限是 200 层。入睡前少想一点"我要写递归"。

**能用 Python 调用 `.dc` 吗？**
目前没有 Python 绑定。dc 是一个 C++ 库，可以直接嵌入 C++ 程序，见 [扩展指南](./docs/extending.md)。

**这是一门真正的编程语言吗？**
这是一个梗。请不要部署到生产环境，除非你的生产环境也在梦里。

**有 linter 吗？**
有。它在你的潜意识里，叫做"第二天早上的羞耻感"。

## 路线图

- [x] 确定后缀名 `.dc`，并写出第一段可运行的梦
- [x] `dc sleep` 与 `dc wake`
- [x] 模块导入与标准库（`math`、`dream`）
- [ ] 梦中的异步 I/O
- [ ] 解决"醒来后忘记梦的内容"（目前无解）

## Star History

<a href="https://www.star-history.com/?repos=alone8199%2Fdream-coding&type=date&legend=top-left">
 <picture>
   <source media="(prefers-color-scheme: dark)" srcset="https://api.star-history.com/chart?repos=alone8199/dream-coding&type=date&theme=dark&legend=top-left" />
   <source media="(prefers-color-scheme: light)" srcset="https://api.star-history.com/chart?repos=alone8199/dream-coding&type=date&legend=top-left" />
   <img alt="Star History Chart" src="https://api.star-history.com/chart?repos=alone8199/dream-coding&type=date&legend=top-left" />
 </picture>
</a>

## 贡献与许可

欢迎贡献，请先阅读 [CONTRIBUTING.md](./CONTRIBUTING.md)。注意：提交的代码最好是在梦里写的。

许可证见 [LICENSE](./LICENSE)。

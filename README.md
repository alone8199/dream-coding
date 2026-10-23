# Dream Coding

[中文](./README.md) | [English](./README.en.md)

> 编译于潜意识，运行于不存在。
> 在梦里写代码，醒来只剩 bug 和诗。

**Dream Coding** 是一种在睡眠中完成开发的技术。你不敲键盘，只是在睡前把想法写下来，让潜意识去编译和运行它。醒来后看到的是一个跑不起来的乌托邦，以及一行终端输出：

```
Segmentation fault (dream dumped)
```

这就是正常的结果。

## 流程

1. **白天**：打开梦境脚本，读一遍，然后关掉它。
2. **入睡**：`dc sleep`，进入 REM 周期，不少于 7 小时。
3. **醒来**：`dc wake`，看到 segfault，不要 debug。
4. **解梦**：如果还记得梦里的内容，写下来；不记得也没关系。
5. **提交**：只在入睡时提交，醒来后假装什么都没发生。

vibe coding 的尽头是 Stack Overflow，dream coding 的尽头是弗洛伊德。

## 工具

`dc` 是 Dream Coding 的配套执行工具，用 C++17 写成，负责把梦境脚本（`.dc` 文件）交给潜意识执行。

```
$ dc run examples/fib.dc
fib(0..9): 0, 1, 1, 2, 3, 5, 8, 13, 21, 34
counter after 3 calls: 3
```

## 文档

- [使用说明](./docs/zh/usage.md)：构建、命令、梦境脚本放在哪里
- [脚本参考](./docs/zh/scripts.md)：`.dc` 的语法与标准库
- [扩展 dc](./docs/zh/extending.md)：写模块、注册函数与模块
- [与 vibe coding 的对比](./docs/zh/vibe-vs-dream.md)

## 项目结构

```
dream-coding/
├── src/                    # dc 的实现与入口梦境 main.dc
├── examples/               # 示例梦境（CI 会运行它们）
├── tests/                  # 单元测试与解释器测试
├── docs/
│   ├── zh/                 # 中文文档
│   └── en/                 # English docs
├── pseudocode/             # 早期的伪代码草稿，只用来做梦
├── README.md / README.en.md
├── CHANGELOG.md / CHANGELOG.en.md
├── CONTRIBUTING.md / CONTRIBUTING.en.md
└── LICENSE
```

## 贡献

请先阅读 [CONTRIBUTING.md](./CONTRIBUTING.md)。注意：提交的代码最好是在梦里写的。

## 许可证

见 [LICENSE](./LICENSE)。

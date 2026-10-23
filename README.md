# Dream Coding

> 编译于潜意识，运行于不存在。
> 在梦里写代码，醒来只剩 bug 和诗。

**Dream Coding** 是一门在睡眠中完成开发的编程范式。源文件后缀为 `.dc`，编译器运行在你的 REM 周期里，程序在梦中已经完美执行过，醒来后你得到的只是它的尸体。

```
$ dc run main.dc
Compiling in subconscious... done
Running in nonexistent...   done
Waking up...                 done

Segmentation fault (dream dumped)
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

完整对比见 [docs/vibe-vs-dream.md](./docs/vibe-vs-dream.md)。

## 快速开始

### 1. 准备环境

你需要：

- 一张床
- 至少 7 小时的睡眠时间（不足则可能产生 `TimeoutError: sleep too short`）
- 褪黑素一粒（可选，但强烈推荐）

### 2. 写代码

在白天打开 `src/main.dc`，阅读一遍，然后关掉它。

### 3. 入睡

```
$ dc sleep
Entering REM cycle...
zzz...
```

### 4. 醒来

```
$ dc wake
Segmentation fault (dream dumped)
```

这就是正常的输出。请不要 debug，请去解梦。

## 项目结构

```
dream-coding/
├── src/
│   ├── main.dc            # 程序入口，梦的开始
│   ├── stages/
│   │   ├── compile.dc     # 在潜意识中编译
│   │   ├── run.dc         # 在不存在中运行
│   │   ├── debug.dc       # 解梦（不是调试）
│   │   ├── commit.dc      # 入睡即提交
│   │   └── deploy.dc      # 醒来并假装什么都没发生
│   └── dream/
│       ├── rem.dc         # REM 周期调度器
│       └── memory.dc      # 海马体内存管理
├── tests/
│   └── test_wake.dc       # 唯一的测试：能不能醒来
├── docs/
│   └── vibe-vs-dream.md   # 与 vibe coding 的对比
├── CHANGELOG.md
├── CONTRIBUTING.md
├── LICENSE
└── README.md
```

## 语法速览

`.dc` 文件的语法借鉴了 C 和梦境的不确定性：

```dc
// 声明一个梦
dream Hello {
    // 变量可以是任何东西，只要你梦到了
    let greeting = "你好，醒醒";

    // 函数在梦里会被自动调用
    fn wake() {
        print(greeting);
    }
}

// 入口：闭眼
sleep main() {
    Hello.wake();
}
```

输出（醒来后看到的）：

```
Segmentation fault (dream dumped)
```

更多语法见 `src/` 下的各个阶段文件。

## 常见问题

**Q：程序跑不通怎么办？**
A：它在梦里跑通过了。请确认你的醒来时机是否正确。

**Q：为什么编译器报错说 "stack overflow in subconscious"？**
A：你梦里的递归太深了。建议在入睡前少想一点"我要写递归"。

**Q：能用 Python 调用 `.dc` 吗？**
A：可以，但你需要在睡前用 Python 写好接口，然后在梦里调用它。醒来后你会发现接口已经存在了，只是没有实现。

**Q：这是一个真正的编程语言吗？**
A：这是一个梗。请不要把它部署到生产环境，除非你的生产环境也在梦里。

**Q：有 linter 吗？**
A：有。它在你的潜意识里，叫做"第二天早上的羞耻感"。

## 路线图

- [x] 确定后缀名：`.dc`
- [x] 写下第一段伪代码
- [ ] 实现 `dc sleep` 命令
- [ ] 实现 `dc wake` 命令（目前只输出 segfault）
- [ ] 支持梦中的异步 I/O
- [ ] 解决 "醒来后忘记梦的内容" 问题（目前无解）

## 贡献

欢迎贡献。请先阅读 [CONTRIBUTING.md](./CONTRIBUTING.md)。注意：提交的代码最好是在梦里写的。

## 构建

`dc` 是用 C++17 写的运行器，使用 CMake 构建。

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

构建产物是 `build/dc`（Windows 上是 `build/Release/dc.exe`）。

### 命令

| 命令 | 作用 |
|---|---|
| `dc sleep [hours]` | 进入 REM 周期，默认 8 小时，不足 7 小时会报 `TimeoutError` |
| `dc run <file.dc>` | 在潜意识中编译，在不存在中运行 |
| `dc wake` | 醒来，输出 segfault |
| `dc version` | 打印版本 |
| `dc help` | 打印帮助 |

### GitHub Actions

`.github/workflows/build.yml` 会在 Ubuntu、macOS、Windows 上构建并测试 `dc`，然后执行一次冒烟测试，并上传构建产物。推送 `v*` 标签时会自动创建 Release。

## 许可证

见 [LICENSE](./LICENSE)。

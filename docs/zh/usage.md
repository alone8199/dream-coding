# 使用说明

`dc` 是 Dream Coding 的执行工具。本文介绍怎么构建、怎么用命令行，以及梦境脚本放在哪里。

- 脚本语法见 [scripts.md](./scripts.md)
- 扩展 dc 见 [extending.md](./extending.md)
- 与 vibe coding 的对比见 [vibe-vs-dream.md](./vibe-vs-dream.md)

## 构建

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

产物是 `build/dc`（Windows 上是 `build/Release/dc.exe`）。

## 命令

| 命令 | 作用 |
|---|---|
| `dc sleep [hours]` | 进入 REM 周期，默认 8 小时，不足 7 小时会超时 |
| `dc wake` | 醒来，输出 segfault |
| `dc run <file.dc> [args...]` | 在潜意识中编译，在不存在中运行；额外参数通过 `argv` 读取 |
| `dc check <file.dc>` | 只做语法检查，不运行 |
| `dc eval "<code>"` | 运行一行代码 |
| `dc version` / `dc help` | 版本与帮助 |

## 梦境脚本放在哪里

- `src/main.dc`：项目的入口梦境，`dc run src/main.dc` 会执行它。
- `examples/`：示例脚本，CI 会检查它们的语法和输出。
- 其他 `.dc` 文件可以放在任何地方，用 `dc run` 指定路径即可。

## 出错时

脚本出错时，会带上文件名和行号，输出 segfault，并以退出码 4 结束：

```
$ dc run bad.dc
bad.dc:2: NameError: name 'zzz' is not defined
Segmentation fault (dream dumped)
```

## 常见问题

**Q：程序跑不通怎么办？**
A：它在梦里跑通过了。请确认你的醒来时机是否正确。

**Q：为什么报 `RecursionError: stack overflow in subconscious`？**
A：递归太深了，上限是 200 层。建议在入睡前少想一点"我要写递归"。

# 贡献指南

[中文](./CONTRIBUTING.md) | [English](./CONTRIBUTING.en.md)

感谢你愿意为 Dream Coding 贡献代码。在开始之前，请先了解以下规则。

## 基本原则

1. **在梦里写代码。** 如果你醒着写，请在睡前至少闭眼五分钟。
2. **不要 debug。** 出错时输出 segfault，然后去解梦。
3. **不要强行 commit。** 请先入睡，醒来后如果还记得，再提交。
4. **不要 deploy。** 醒来后请假装什么都没发生。

## 提交流程

1. Fork 本仓库。
2. 创建分支，分支名建议使用梦的主题，例如 `feat/flying-dream` 或 `fix/falling-off-cliff`。
3. 修改 `src/`、`examples/` 或 `docs/` 下的内容。文档修改请同时更新中文与英文版本。
4. 运行 `dc sleep`，等待 REM 周期结束。
5. 运行 `dc wake`，如果输出包含 `Segmentation fault (dream dumped)`，说明你的梦已经成功运行。
6. 运行 `ctest --test-dir build --output-on-failure`，确认测试通过。
7. 提交 Pull Request，并在描述里写下你梦到了什么。

## Commit message 格式

```
<类型>: <梦的内容>

类型可选：
- dream：新增梦境
- nightmare：修复噩梦（即 bug）
- wake：修改醒来的逻辑
- sleep：修改入睡的逻辑
```

示例：

```
dream: 梦见自己在写一个永远不会运行的编译器
nightmare: 修复在考试时发现忘记带代码的问题
```

## 代码风格

- C++ 缩进使用两个空格（已有代码的 `src/dc/` 使用两个空格，`tests/runner_test.cpp` 使用四个空格，新代码请跟随所在文件）。
- 注释使用中文，关键字与标识符使用英文。
- 新增的 `.dc` 示例需要能通过 `dc check`，并且输出能被 CI 检查。

## 审查

PR 会由 reviewer 在他们自己的梦里审查。如果 reviewer 在梦里没有通过，请耐心等待醒来后的第二轮审查。

## 行为准则

请保持友善。梦里的事情不应该伤害醒着的人。

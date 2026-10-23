# Changelog

[中文](./CHANGELOG.md) | [English](./CHANGELOG.en.md)

本项目的所有重要变更都会记录在这里。格式参考 [Keep a Changelog](https://keepachangelog.com/zh-CN/1.1.0/)。

## [Unreleased]

### Added
- dc 解释器：词法分析、递归下降语法分析、树遍历求值，支持变量、函数、闭包、递归、`if` / `while` / `for`、`break` / `continue`、列表、字典、字符串方法、`in` 运算符。
- 错误带文件名和行号，运行时错误以退出码 4 结束。
- 模块系统：`import name;` 加载标准库或注册的模块，`import "path.dc";` 加载文件模块，以 `_` 开头的名字不导出。
- 标准库：`math`（`sqrt`、`pow`、`min`、`max` 等）、`dream`（`sleep`、`wake`）。
- 全局函数：`print`、`len`、`str`、`num`、`int`、`type`、`range`、`list`、`assert`、`raise`、`time`。
- 扩展接口：`Interpreter::define_function`、`Interpreter::define_global`、`Interpreter::register_module`。
- 命令：`dc run`（支持 `argv`）、`dc check`、`dc eval`。
- `examples/`：示例脚本，CI 会检查它们的输出。
- 文档按语言拆分：`docs/zh/` 与 `docs/en/`。
- `README.en.md`，以及 `CONTRIBUTING.en.md`、`CHANGELOG.en.md`。
- `tests/interp_test.cpp`：66 项解释器测试。

### Changed
- `src/main.dc` 改为可以运行的程序；早期的伪代码移到 `pseudocode/`。
- `dc run` 不再只检查括号，而是真正执行脚本。
- 文档与 README 的叙述改为以 Dream Coding 技术为主，`dc` 是配套执行工具。

### Previously added
- `dc` 命令行运行器（C++17）：`sleep`、`run`、`wake`、`version`、`help`。
- `src/dc/runner.{hpp,cpp}`：运行器核心，包含花括号检查与命令分发。
- `tests/runner_test.cpp`：运行器的单元测试，使用 CTest 运行。
- `CMakeLists.txt`：跨平台构建脚本。
- `.github/workflows/build.yml`：在 Ubuntu、macOS、Windows 上构建、测试并上传产物；推送 `v*` 标签时发布 Release。
- `src/dream/rem.dc`：REM 周期调度器的初版设计。
- `src/dream/memory.dc`：海马体内存管理（目前只会忘记）。
- `tests/test_wake.dc`：唯一的测试用例。

### Changed (earlier)
- 将源文件后缀从 `.dream` 改为 `.dc`，更短，也更像一个真正的编程语言。

## [0.1.0] - 2026-10-23

### Added
- 项目初始化。
- `src/main.dc`：程序入口，梦的开始。
- `src/stages/`：compile、run、debug、commit、deploy 五个阶段。
- `docs/vibe-vs-dream.md`：与 vibe coding 的对比。
- `README.md` 与 `CONTRIBUTING.md`。

### Known Issues
- 醒来后忘记梦的内容（无解）。
- `dc wake` 只会输出 segfault，不会输出其他东西。

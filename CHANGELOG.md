# Changelog

本项目的所有重要变更都会记录在这里。格式参考 [Keep a Changelog](https://keepachangelog.com/zh-CN/1.1.0/)。

## [Unreleased]

### Added
- `dc` 命令行运行器（C++17）：`sleep`、`run`、`wake`、`version`、`help`。
- `src/dc/runner.{hpp,cpp}`：运行器核心，包含花括号检查与命令分发。
- `tests/runner_test.cpp`：运行器的单元测试，使用 CTest 运行。
- `CMakeLists.txt`：跨平台构建脚本。
- `.github/workflows/build.yml`：在 Ubuntu、macOS、Windows 上构建、测试并上传产物；推送 `v*` 标签时发布 Release。
- `src/dream/rem.dc`：REM 周期调度器的初版设计。
- `src/dream/memory.dc`：海马体内存管理（目前只会忘记）。
- `tests/test_wake.dc`：唯一的测试用例。

### Changed
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

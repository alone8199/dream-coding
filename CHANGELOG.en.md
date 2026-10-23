# Changelog

[中文](./CHANGELOG.md) | [English](./CHANGELOG.en.md)

All notable changes to this project are recorded here. The format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

## [Unreleased]

### Added
- The dc interpreter: lexer, recursive-descent parser, and tree-walking evaluation. Supports variables, functions, closures, recursion, `if` / `while` / `for`, `break` / `continue`, lists, dicts, string methods, and the `in` operator.
- Errors include the file name and line number. Runtime errors exit with code 4.
- Module system: `import name;` loads a standard or registered module, `import "path.dc";` loads a file module. Names starting with `_` are not exported.
- Standard library: `math` (`sqrt`, `pow`, `min`, `max`, and more) and `dream` (`sleep`, `wake`).
- Built-in functions: `print`, `len`, `str`, `num`, `int`, `type`, `range`, `list`, `assert`, `raise`, `time`.
- Extension API: `Interpreter::define_function`, `Interpreter::define_global`, `Interpreter::register_module`.
- Commands: `dc run` (with `argv`), `dc check`, `dc eval`.
- `examples/`: sample scripts. CI checks their output.
- Docs split by language: `docs/zh/` and `docs/en/`.
- `README.en.md`, `CONTRIBUTING.en.md`, `CHANGELOG.en.md`.
- `tests/interp_test.cpp`: 66 interpreter checks.

### Changed
- `src/main.dc` is now a runnable program. Early pseudocode moved to `pseudocode/`.
- `dc run` no longer only checks braces; it executes the script.
- Docs and READMEs now lead with the Dream Coding technique. `dc` is the companion runner.

### Previously added
- The `dc` command-line runner (C++17): `sleep`, `run`, `wake`, `version`, `help`.
- `src/dc/runner.{hpp,cpp}`: runner core with brace checking and command dispatch.
- `tests/runner_test.cpp`: runner unit tests, run with CTest.
- `CMakeLists.txt`: cross-platform build script.
- `.github/workflows/build.yml`: builds and tests on Ubuntu, macOS, and Windows, uploads artifacts, and publishes a release on `v*` tags.
- `src/dream/rem.dc`: first design of the REM cycle scheduler.
- `src/dream/memory.dc`: hippocampus memory management (currently only forgets).
- `tests/test_wake.dc`: the only test case.

### Changed (earlier)
- Source suffix changed from `.dream` to `.dc`: shorter, and more like a real programming language.

## [0.1.0] - 2026-10-23

### Added
- Project initialized.
- `src/main.dc`: the entry point, where the dream begins.
- `src/stages/`: five stages: compile, run, debug, commit, deploy.
- `docs/vibe-vs-dream.md`: comparison with vibe coding.
- `README.md` and `CONTRIBUTING.md`.

### Known Issues
- You forget the dream after waking (no known fix).
- `dc wake` only prints the segfault and nothing else.

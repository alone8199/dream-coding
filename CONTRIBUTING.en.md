# Contributing

[中文](./CONTRIBUTING.md) | [English](./CONTRIBUTING.en.md)

Thank you for wanting to contribute to Dream Coding. Please read the rules below before you start.

## Core principles

1. **Write code in your dreams.** If you write while awake, close your eyes for at least five minutes before bed.
2. **Do not debug.** When something fails, accept the segfault and go interpret the dream.
3. **Do not force a commit.** Sleep first. If you still remember the code after waking, commit then.
4. **Do not deploy.** After waking, pretend nothing happened.

## Workflow

1. Fork the repository.
2. Create a branch. Name it after a dream theme, for example `feat/flying-dream` or `fix/falling-off-cliff`.
3. Change things under `src/`, `examples/`, or `docs/`. Documentation changes must update both the Chinese and English versions.
4. Run `dc sleep` and wait for the REM cycle to finish.
5. Run `dc wake`. If the output contains `Segmentation fault (dream dumped)`, your dream ran successfully.
6. Run `ctest --test-dir build --output-on-failure` and make sure the tests pass.
7. Open a pull request, and describe what you dreamed in the description.

## Commit message format

```
<type>: <what you dreamed>

Allowed types:
- dream: a new dream
- nightmare: fixing a nightmare (a bug)
- wake: changing the waking logic
- sleep: changing the falling-asleep logic
```

Examples:

```
dream: dreamt of writing a compiler that never runs
nightmare: fixed forgetting to bring the code to the exam
```

## Code style

- C++ indentation: `src/dc/` uses two spaces, `tests/runner_test.cpp` uses four. Follow the style of the file you are editing.
- Comments are written in Chinese; keywords and identifiers are in English.
- New `.dc` examples must pass `dc check`, and their output must be checkable by CI.

## Review

Reviewers review pull requests in their own dreams. If a reviewer does not pass in their dream, be patient and wait for a second review after they wake up.

## Code of conduct

Be kind. What happens in dreams should not hurt people who are awake.

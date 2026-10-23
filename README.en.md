# Dream Coding

[中文](./README.md) | [English](./README.en.md)

> Compiled in the subconscious, run in nonexistence.
> Write code in your dreams; wake up to nothing but bugs and poetry.

**Dream Coding** is a technique for doing development while you sleep. You don't touch the keyboard. You write your ideas down before bed and let your subconscious compile and run them. When you wake up, you find a utopia that doesn't run, and one line in the terminal:

```
Segmentation fault (dream dumped)
```

That is the expected result.

## Workflow

1. **Daytime**: open the dream script, read it once, then close it.
2. **Sleep**: `dc sleep`. Enter the REM cycle for at least 7 hours.
3. **Wake up**: `dc wake`. See the segfault. Do not debug.
4. **Interpret**: if you remember the dream, write it down. If not, that is fine too.
5. **Commit**: only commit while falling asleep. After waking up, pretend nothing happened.

vibe coding ends at Stack Overflow. Dream coding ends at Freud.

## Tool

`dc` is the companion runner for Dream Coding. It is written in C++17 and hands `.dc` dream scripts to the subconscious for execution.

```
$ dc run examples/fib.dc
fib(0..9): 0, 1, 1, 2, 3, 5, 8, 13, 21, 34
counter after 3 calls: 3
```

## Docs

- [Usage](./docs/en/usage.md): build, commands, where dream scripts live
- [Script reference](./docs/en/scripts.md): `.dc` syntax and the standard library
- [Extending dc](./docs/en/extending.md): write modules, register functions and modules
- [vibe coding vs dream coding](./docs/en/vibe-vs-dream.md)

## Project layout

```
dream-coding/
├── src/                    # dc implementation and the entry dream main.dc
├── examples/               # sample dreams (CI runs them)
├── tests/                  # unit tests and interpreter tests
├── docs/
│   ├── zh/                 # 中文文档
│   └── en/                 # English docs
├── pseudocode/             # early pseudocode drafts, for dreaming only
├── README.md / README.en.md
├── CHANGELOG.md / CHANGELOG.en.md
├── CONTRIBUTING.md / CONTRIBUTING.en.md
└── LICENSE
```

## Contributing

Please read [CONTRIBUTING.md](./CONTRIBUTING.en.md) first. Note: code is best written in a dream.

## License

See [LICENSE](./LICENSE).

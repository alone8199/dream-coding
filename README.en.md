# Dream Coding

[中文](./README.md) | [English](./README.en.md)

> Compiled in the subconscious, run in nonexistence.
> Write code in your dreams; wake up to nothing but bugs and poetry.

**Dream Coding** is a technique for doing development while you sleep. You write the dream by day, let it run at night, and when you wake up you have one line of segfault and a pile of feelings to interpret. `dc` is the command-line tool that makes this workflow actually runnable.

```
$ dc run dreams/fib.dc
fib(0..9): 0, 1, 1, 2, 3, 5, 8, 13, 21, 34
counter after 3 calls: 3
```

## A day in the dream cycle

1. **Write the dream by day.** Open `dreams/main.dc`, read it once, then close it.
2. **Fall asleep.** Run `dc sleep`. It records the sleep and checks the duration (eight hours by default; less than seven hours is an error).
3. **Wake up.** Run `dc wake`, which prints `Segmentation fault (dream dumped)`. That is the normal output. Don't debug.
4. **Interpret the dream.** Recall what happened in the dream, compare it with the script, and decide how it should change.

```
$ dc sleep
Entering REM cycle...
zzz... (8h)

$ dc wake
Waking up...
Segmentation fault (dream dumped)
```

## Quick start

You need a C++17 compiler and CMake.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

The binary is `build/dc` (on Windows: `build/Release/dc.exe`). The everyday commands are `run`, `check`, `eval`, `sleep` and `wake`. Full details are in [docs/usage.en.md](./docs/usage.en.md).

## Dream scripts

`.dc` scripts use a notation close to Python and C: blocks use braces, and semicolons are optional. It is a way to hand your ideas to the subconscious, not a language specification for others to maintain.

```dc
import dream;

fn fib(n) {
    if n < 2 { return n; }
    return fib(n - 1) + fib(n - 2);
}

print("fib(10) =", fib(10));
dream.wake();
```

Output (what you see after waking up):

```
fib(10) = 55
Segmentation fault (dream dumped)
```

## Documentation

| What you want to know | Where to look |
|---|---|
| Learning the language from scratch | [Learn Dream Coding from Scratch](./docs/tutorial.en.md) |
| How to use the commands, and what the exit codes mean | [Usage](./docs/usage.en.md) |
| The full dream notation | [Script reference](./docs/scripts.en.md) |
| How to extend `dc` from C++ | [Extending](./docs/extending.en.md) |
| How this differs from vibe coding | [Comparison](./docs/vibe-vs-dream.en.md) |
| Interpreter and implementation details | [Technical articles](./docs/articles/README.en.md) (36 articles) |
| Something less formal | [Essays](./docs/essays/README.en.md) (poems, monologues, dialogue, a parable, a glossary) |

## Project layout

```
dream-coding/
├── dreams/          dreams: the entry dream main.dc and samples (CI runs them)
├── runtime/         source and tests of dc
├── docs/            documentation: usage, notation, extending, comparison, articles, essays
├── pseudocode/      early pseudocode drafts, for dreaming only
├── README.md        Chinese README
├── README.en.md     this file (English)
├── CHANGELOG.md     change log
├── CONTRIBUTING.md  contribution guide
└── LICENSE
```

## FAQ

**My program doesn't run. What do I do?**
It ran perfectly in your dream. Check that you woke up at the right time.

**Why does it say `RecursionError: stack overflow in subconscious`?**
Your recursion went too deep in the dream. The limit is 200 frames. Think less about "I'll write a recursion" before bed.

**Can I call `.dc` from Python?**
There is no Python binding yet. dc is a C++ library and can be embedded directly into C++ programs. See [Extending](./docs/extending.en.md).

**Is this a real programming language?**
It's a joke. Please don't deploy it to production, unless your production environment is also in a dream.

**Do you have a linter?**
Yes. It lives in your subconscious and is called "the shame you feel the next morning."

## Roadmap

- [x] Settle on the suffix `.dc`, and write the first runnable dream
- [x] `dc sleep` and `dc wake`
- [x] Module imports and the standard library (`math`, `dream`)
- [ ] Asynchronous I/O in dreams
- [ ] Solve "forgetting the dream after waking up" (no known fix)

## Star History

<a href="https://www.star-history.com/?repos=alone8199%2Fdream-coding&type=date&legend=top-left">
 <picture>
   <source media="(prefers-color-scheme: dark)" srcset="https://api.star-history.com/chart?repos=alone8199/dream-coding&type=date&theme=dark&legend=top-left" />
   <source media="(prefers-color-scheme: light)" srcset="https://api.star-history.com/chart?repos=alone8199/dream-coding&type=date&legend=top-left" />
   <img alt="Star History Chart" src="https://api.star-history.com/chart?repos=alone8199/dream-coding&type=date&legend=top-left" />
 </picture>
</a>

## Contributing and license

Please read [CONTRIBUTING.en.md](./CONTRIBUTING.en.md) first. Note: code is best written in a dream.

See [LICENSE](./LICENSE) for the license.

# Dream Coding

[中文](./README.md) | [English](./README.en.md)

> Compiled in the subconscious, run in nonexistence.
> Write code in your dreams; wake up to nothing but bugs and poetry.

**Dream Coding** is a programming paradigm for doing development while you sleep. Source files end in `.dc`. The compiler runs during your REM cycle. The program has already run perfectly in your dream, and when you wake up all you have is its corpse: one line in the terminal.

```
$ dc run examples/fib.dc
fib(0..9): 0, 1, 1, 2, 3, 5, 8, 13, 21, 34
counter after 3 calls: 3
```

---

## Contents

- [Why Dream Coding](#why-dream-coding)
- [How it differs from vibe coding](#how-it-differs-from-vibe-coding)
- [Quick start](#quick-start)
- [Project layout](#project-layout)
- [Dream notation at a glance](#dream-notation-at-a-glance)
- [FAQ](#faq)
- [Roadmap](#roadmap)
- [License](#license)

## Why Dream Coding

- **Zero keyboard wear**: no need to shake the keyboard, just close your eyes.
- **Unlimited runtime**: no memory limit in dreams, and no OOM.
- **Perfect test coverage**: in a dream, every test passes.
- **Native async**: your callbacks all finish before you wake up.
- **No code review needed**: nobody can review a PR that only exists in your subconscious.

## How it differs from vibe coding

| | vibe coding | dream coding |
|---|---|---|
| Your action | Shake the keyboard and let AI guess your mood | Close the IDE and shut your eyes |
| Output | Shaky code that runs | A utopia that doesn't run |
| Debugging | Go to Stack Overflow | Go interpret your dream with Freud |
| Committing | `git commit -m "fix"` | Fall asleep |
| Deploying | Ship it and pray | Wake up and pretend nothing happened |
| Final destination | Stack Overflow | Freud |

The full comparison is in [docs/en/vibe-vs-dream.md](./docs/en/vibe-vs-dream.md).

## Quick start

### 1. Prepare

You need:

- A bed
- At least 7 hours of sleep (less than that and `dc sleep` reports `TimeoutError: sleep too short`)
- One melatonin tablet (optional, but strongly recommended)
- A C++17 compiler and CMake, to build `dc`

Build:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

The binary is `build/dc` (on Windows: `build/Release/dc.exe`).

### 2. Write code

During the day, open `src/main.dc`, read it once, and close it.

### 3. Fall asleep

```
$ dc sleep
Entering REM cycle...
zzz... (8h)
```

### 4. Wake up

```
$ dc wake
Waking up...
Segmentation fault (dream dumped)
```

That is the normal output. Don't debug. Go interpret the dream.

For the other commands (`run`, `check`, `eval`) and how to run scripts, see [docs/en/usage.md](./docs/en/usage.md).

## Project layout

```
dream-coding/
├── src/
│   ├── main.cpp            # dc entry point
│   ├── main.dc             # the entry dream, where it all begins
│   └── dc/                 # dc implementation: lexer, parser, evaluator, stdlib, CLI
├── examples/               # sample dreams (CI runs them)
├── tests/                  # unit tests and interpreter tests
├── docs/
│   ├── zh/                 # 中文文档：使用说明、脚本参考、扩展、对比
│   └── en/                 # English docs: usage, script reference, extending, comparison
├── pseudocode/             # early pseudocode drafts, for dreaming only
├── README.md / README.en.md
├── CHANGELOG.md / CHANGELOG.en.md
├── CONTRIBUTING.md / CONTRIBUTING.en.md
└── LICENSE
```

## Dream notation at a glance

`.dc` dream scripts use a notation close to Python and C. Blocks use braces, and semicolons are optional. It is a way to hand your ideas to the subconscious, not a language specification for others to maintain:

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

The full notation is in [docs/en/scripts.md](./docs/en/scripts.md).

## FAQ

**Q: My program doesn't run. What do I do?**
A: It ran perfectly in your dream. Check that you woke up at the right time.

**Q: Why does it say `RecursionError: stack overflow in subconscious`?**
A: Your recursion went too deep in the dream. The limit is 200 frames. Try to think less about "I'll write a recursion" before bed.

**Q: Can I call `.dc` from Python?**
A: There is no Python binding yet. dc is a C++ library and can be embedded directly into C++ programs. See [docs/en/extending.md](./docs/en/extending.md).

**Q: Is this a real programming language?**
A: It's a joke. Please don't deploy it to production, unless your production environment is also in a dream.

**Q: Do you have a linter?**
A: Yes. It lives in your subconscious and is called "the shame you feel the next morning."

## Roadmap

- [x] Settle on the suffix: `.dc`
- [x] Write the first pseudocode, and turn it into a runnable script
- [x] Implement the `dc sleep` command
- [x] Implement the `dc wake` command (prints the segfault)
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

## Contributing

Please read [CONTRIBUTING.en.md](./CONTRIBUTING.en.md) first. Note: code is best written in a dream.

## License

See [LICENSE](./LICENSE).

# Dream Coding

[中文](./README.md) | [English](./README.en.md)

> Compiled in the subconscious, run in nonexistence.
> Write code in your dreams; wake up to nothing but bugs and poetry.

**Dream Coding** is a technique for doing development while you sleep. You write the dream by day, let it run at night, and when you wake up you have one line of segfault and a pile of feelings to interpret.

```
$ dc run dreams/fib.dc
fib(0..9): 0, 1, 1, 2, 3, 5, 8, 13, 21, 34
counter after 3 calls: 3
```

---

## Contents

- [Quick start](#quick-start)
- [About the technique](#about-the-technique)
- [Reading](#reading)
- [Dream notation at a glance](#dream-notation-at-a-glance)
- [Project layout](#project-layout)
- [FAQ](#faq)
- [Roadmap](#roadmap)
- [Contributing](#contributing)
- [License](#license)

## Quick start

### 1. Prepare

- A bed
- At least 7 hours of sleep (less than that and `dc sleep` reports `TimeoutError: sleep too short`)
- A C++17 compiler and CMake, to build `dc`

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

The binary is `build/dc` (on Windows: `build/Release/dc.exe`).

### 2. Write the dream by day

Open `dreams/main.dc`, read it once, then close it.

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

For the other commands (`run`, `check`, `eval`), see [docs/en/usage.md](./docs/en/usage.md).

## About the technique

Dream Coding comes down to three things: write your intent by day, let it execute at night, interpret the result after waking. It is not a programming language but a way of working. `dc` is the tool that makes that way of working runnable.

| | vibe coding | dream coding |
|---|---|---|
| Your action | Shake the keyboard and let AI guess your mood | Close the IDE and shut your eyes |
| Output | Shaky code that runs | A utopia that doesn't run |
| Debugging | Go to Stack Overflow | Go interpret your dream with Freud |
| Committing | `git commit -m "fix"` | Fall asleep |
| Deploying | Ship it and pray | Wake up and pretend nothing happened |

The full comparison is in [docs/en/vibe-vs-dream.md](./docs/en/vibe-vs-dream.md).

## Reading

The writing in this project falls into three groups:

- **Technical articles**: 36 of them, from the principles of dream coding to the lexer, syntax tree, value representation and module system of `dc`. Entry point: [docs/en/articles/](./docs/en/articles/README.md).
- **Script and extension reference**: the full dream notation, and how to register native functions and modules in C++. See [docs/en/scripts.md](./docs/en/scripts.md) and [docs/en/extending.md](./docs/en/extending.md).
- **Essays**: poems, monologues, dialogues, a parable and a glossary. They have little to do with the technique, or rather, they circle around it. Entry point: [docs/en/essays/](./docs/en/essays/README.md).

## Dream notation at a glance

`.dc` dream scripts use a notation close to Python and C. Blocks use braces, and semicolons are optional. It is a way to hand your ideas to the subconscious, not a language specification for others to maintain.

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

## Project layout

```
dream-coding/
├── dreams/                 # dreams: the entry dream main.dc and sample dreams (CI runs them)
├── runtime/                # dc: the tool that lets dreams run in the subconscious
│   ├── src/                # main.cpp and the implementation under dc/
│   └── tests/              # unit tests and interpreter tests
├── docs/
│   ├── zh/                 # Chinese docs
│   │   ├── articles/       # technical articles (36)
│   │   └── essays/         # essays
│   └── en/                 # English: usage, script reference, extending, comparison
│       ├── articles/       # technical articles (36)
│       └── essays/         # essays
├── pseudocode/             # early pseudocode drafts, for dreaming only
├── README.md / README.en.md
├── CHANGELOG.md / CHANGELOG.en.md
├── CONTRIBUTING.md / CONTRIBUTING.en.md
└── LICENSE
```

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

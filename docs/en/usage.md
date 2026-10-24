# Usage

`dc` is the tool that runs Dream Coding scripts. This page covers building, the command line, and where dream scripts live.

- Script syntax: [scripts.md](./scripts.md)
- Extending dc: [extending.md](./extending.md)
- Comparison with vibe coding: [vibe-vs-dream.md](./vibe-vs-dream.md)

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

The binary is `build/dc` (on Windows: `build/Release/dc.exe`).

## Commands

| Command | What it does |
|---|---|
| `dc sleep [hours]` | Enter the REM cycle. Defaults to 8 hours; under 7 hours it times out |
| `dc wake` | Wake up and print the segfault |
| `dc run <file.dc> [args...]` | Compile in the subconscious, run in nonexistent. Extra arguments are available as `argv` |
| `dc check <file.dc>` | Parse only, do not run |
| `dc eval "<code>"` | Run one line of code |
| `dc version` / `dc help` | Version and help |

## Where dream scripts live

- `dreams/main.dc`: the entry dream of this project. `dc run dreams/main.dc` runs it.
- `dreams/`: sample scripts. CI checks their syntax and output.
- Any other `.dc` file can live anywhere; pass its path to `dc run`.

## When something goes wrong

On error, dc prints the file name and line number, then the segfault, and exits with code 4:

```
$ dc run bad.dc
bad.dc:2: NameError: name 'zzz' is not defined
Segmentation fault (dream dumped)
```

## FAQ

**Q: The program doesn't run. What now?**
A: It ran fine in your dream. Check whether you woke up at the right time.

**Q: Why `RecursionError: stack overflow in subconscious`?**
A: The recursion is too deep; the limit is 200 frames. Try thinking less about "I'll write a recursion" before sleeping.

# Learn Dream Coding from Scratch

[中文](./tutorial.md) | [English](./tutorial.en.md)

Project: <https://github.com/alone8199/dream-coding>

This tutorial takes you from zero to writing and running `.dc` scripts with `dc`. Every example has been run, and the output shown is what you will see in your terminal.

## 1. Setup

You need a C++17 compiler and CMake.

```bash
git clone https://github.com/alone8199/dream-coding.git
cd dream-coding
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

After the build, `build/dc` (on Windows: `build/Release/dc.exe`) is the command-line tool. Add it to your `PATH`, or run it by its full path.

`dc` has seven commands:

| Command | What it does |
|---|---|
| `dc run <file>` | Run a `.dc` script |
| `dc check <file>` | Check syntax only, without running |
| `dc eval '<code>'` | Run one line of code |
| `dc sleep [hours]` | Record falling asleep. Defaults to eight hours; less than seven is an error |
| `dc wake` | Record waking up, and print the segfault |
| `dc version` | Show the version |
| `dc help` | Show help |

## 2. Your First Dream

Create `hello.dc`:

```dc
import dream;

print("hello, dream");
dream.wake();
```

Run it:

```bash
dc run hello.dc
```

Output:

```
hello, dream
Segmentation fault (dream dumped)
```

The last line is normal. It means the dream has finished running. After waking, you interpret the dream rather than debug it.

Two notes: semicolons at the end of statements are optional, but writing them is clearer. `import dream;` is required to use `dream.sleep` and `dream.wake`.

## 3. Variables

Declare a variable with `let`, and change it with plain assignment:

```dc
let name = "dream";
let count = 1;
count = count + 1;
print("hello,", name, count);
```

Output: `hello, dream 2`

`print` separates its arguments with spaces.

## 4. Basic Types

| Type | Example | Notes |
|---|---|---|
| Number | `42`, `3.14` | All numbers are floating point |
| String | `"dc"` | Double quotes. Escapes: `\n`, `\t`, `\r`, `\0`, `\"`, `\\` |
| Boolean | `true`, `false` | |
| Empty | `null` | |
| List | `[1, 2, 3]` | |
| Dict | `{"ann": 30}` | Keys are strings. Iteration follows sorted key order |

Strings concatenate with `+`, but different types cannot be added directly. Convert with `str()` first:

```dc
print("ab" + "cd");          # abcd
print("x" + str(1));         # x1
```

## 5. Lists and Dicts

```dc
let nums = [3, 1, 2];
nums.push(9);
print(len(nums), nums.pop());   # 4 9

let ages = {"ann": 30, "bob": 25};
print(ages.keys());             # ["ann", "bob"]
print(ages.get("bob"));         # 25
print(ages.has("carol"));       # false
```

Common list methods are `push`, `pop`, `reverse`, and `join`. Common string methods are `upper`, `lower`, `strip`, `split`, `replace`, `startswith`, and `endswith`.

## 6. Conditions and Loops

```dc
let i = 0;
while i < 3 {
    i = i + 1;
    if i == 2 { continue; }
    print("i =", i);
}
```

Output:

```
i = 1
i = 3
```

`for` iterates over a list, a string (byte by byte), or a dict (its keys):

```dc
for n in range(1, 6) {
    print(n);
}
```

`range(a, b)` returns the numbers from `a` up to `b - 1`. You can also write `range(b)` or `range(a, b, step)`.

`break` ends the innermost loop, and `continue` jumps to the next iteration. Logical operators are `and`, `or`, and `not`. Membership uses `in`:

```dc
print(2 in [1, 2, 3]);                  # true
print(true and not false);              # true
```

## 7. Functions

```dc
fn square(x) {
    return x * x;
}
print(square(7));    # 49
```

Functions can recurse:

```dc
fn fact(n) {
    if n <= 1 { return 1; }
    return n * fact(n - 1);
}
print(fact(10));     # 3628800
```

Functions are also values. An unnamed `fn(...) { ... }` is an anonymous function, and it can capture variables from its surrounding scope:

```dc
fn make_counter() {
    let count = 0;
    return fn() {
        count = count + 1;
        return count;
    };
}

let c = make_counter();
c();
print(c());         # 2
```

Note: the recursion depth limit is 200. Going past it raises `RecursionError: stack overflow in subconscious`.

## 8. Modules

`import` has three forms:

```dc
import dream;                 # standard library: the dream module
import math;                  # standard library: the math module
import "helpers.dc";          # file module: loads helpers.dc from the same folder
```

The `math` members are `pi`, `e`, `sqrt`, `floor`, `ceil`, `abs`, `pow`, `min`, and `max`:

```dc
import math;
print(math.sqrt(16), math.max(4, 9, 2));    # 4 9
```

The `dream` module corresponds to the dream workflow, covered in section 9.

A file module takes its name from the file, without `.dc`. For example, with `lib/helpers.dc`:

```dc
import "lib/helpers.dc";
print(helpers.shown);       # access names as helpers.<name>
```

Names that start with an underscore are private. Accessing one from outside raises an error: `helpers._secret` gives `AttributeError`.

Command-line arguments are available in the global variable `argv`. With `dc run hello.dc a b`, `argv` is `["a", "b"]`.

## 9. The Dream Cycle

The core workflow of Dream Coding is: write the dream by day, fall asleep, wake up, interpret the dream.

On the command line, this corresponds to `dc sleep` and `dc wake`:

```bash
dc sleep
# Entering REM cycle...
# zzz... (8h)

dc wake
# Waking up...
# Segmentation fault (dream dumped)
```

`dc sleep` defaults to eight hours. Less than seven gives `TimeoutError: sleep too short`. `dc wake` prints the segfault, which is the normal result.

Inside a script, the same idea is expressed with the `dream` module:

```dc
import dream;
dream.sleep(8);      # fall asleep; less than 7 hours is an error
# ... code from the dream ...
dream.wake();        # wake up and print the segfault
```

## 10. Errors

When a runtime error happens, `dc` prints the file name, line number, and message, then exits with code 4:

```dc
print(ghost);
```

```
t.dc:1: NameError: name 'ghost' is not defined
Segmentation fault (dream dumped)
```

Common errors:

- `SyntaxError`: a syntax problem. `dc check` can find these early.
- `NameError`: a name that is not defined.
- `TypeError`: an operand of the wrong type.
- `ZeroDivisionError`: division by zero.
- `ImportError`: a module cannot be found, or an import is circular.
- `RecursionError`: recursion too deep.

The full exit code convention is in the [usage guide](./usage.en.md).

## 11. Where to Go Next

- Full syntax reference: [Script reference](./scripts.en.md)
- Extending `dc` from C++: [Extending](./extending.en.md)
- How the interpreter works: [Technical articles](./articles/README.en.md) (36 articles, in English)
- Less formal reading: [Essays](./essays/README.en.md)
- Contributing: [CONTRIBUTING.en.md](../CONTRIBUTING.en.md)

VS Code users can install the companion extension (a `.vsix` package). It provides syntax highlighting, IntelliSense, syntax diagnostics, and `Ctrl+F5` to run a script. See the extension package for installation instructions.

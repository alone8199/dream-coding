# Dream Script Notation Reference

`.dc` dream scripts are written in a notation close to Python and C. Blocks use braces, and semicolons are optional. `dc` hands them to the subconscious to run. This page only records how the notation is written. It is not a language specification.

## Comments

```dc
# this is a comment
// this is also a comment
```

## Values

| Type | Example | Notes |
|---|---|---|
| null | `null` | |
| bool | `true`, `false` | |
| number | `1`, `3.14` | All numbers are double-precision floats |
| string | `"hello\n"` | Escapes: `\n \t \r \0 \" \\` |
| list | `[1, "a", [2]]` | Reference semantics, like Python |
| dict | `{name: "dc", "k": 1}` | Keys are strings; a bare name becomes a string key |
| function | `fn(x) { return x; }` | Closures |

Truthiness: `null`, `false`, `0`, the empty string, the empty list, and the empty dict are false. Everything else is true.

## Variables

```dc
let x = 1;      # define in the current scope
x = x + 1;      # assign to an existing variable (NameError if undefined)
```

Scopes are block-level: `if`, `while`, `for`, and function bodies each open a new scope.

## Operators

| Kind | Operators |
|---|---|
| Arithmetic | `+ - * / %`. `+` also concatenates strings and lists; `*` repeats a string |
| Comparison | `== != < <= > >=` |
| Membership | `x in container` (list items, substrings, dict keys) |
| Logic | `and or not`, or `&& || !`. `and` and `or` short-circuit |
| Unary | `-x`, `not x` |

`/` always gives a float. Dividing by zero raises `ZeroDivisionError`.

## Control flow

```dc
if x > 0 {
    print("positive");
} else if x == 0 {
    print("zero");
} else {
    print("negative");
}

while i < 10 {
    i = i + 1;
    if i == 3 { continue; }
    if i > 6 { break; }
}

for item in [1, 2, 3] {     # works on lists, strings (per character), and dicts (per key)
    print(item);
}
```

Conditions do not need parentheses.

## Functions

```dc
fn add(a, b) {
    return a + b;
}

let square = fn(x) { return x * x; };   # anonymous function

print(add(1, 2), square(4));
```

Functions can nest, and inner functions capture outer variables:

```dc
fn make_counter() {
    let n = 0;
    return fn() {
        n = n + 1;
        return n;
    };
}
```

Recursion is limited to 200 frames. Going deeper raises `RecursionError: stack overflow in subconscious`.

## Containers

```dc
let xs = [1, 2, 3];
xs[0];          # 1
xs[-1];         # 3, negative indexes count from the end
xs[0] = 9;

let d = {a: 1};
d.a;            # 1
d["b"] = 2;
d.b = 3;        # same as d["b"] = 3
```

## Methods

Lists: `push(v)`, `pop()`, `reverse()`, `join(sep)`

Strings: `upper()`, `lower()`, `strip()`, `split(sep)`, `replace(a, b)`, `startswith(p)`, `endswith(p)`

Dicts: `keys()`, `values()`, `has(k)`, `get(k, default)`

## Built-in functions

| Function | What it does |
|---|---|
| `print(...)` | Print arguments separated by spaces |
| `len(x)` | Length of a string, list, or dict |
| `str(x)` / `num(x)` / `int(x)` | Conversions |
| `type(x)` | Type name |
| `range(stop)` / `range(start, stop, step)` | Build a list of numbers |
| `list(x)` | Turn an iterable into a list |
| `assert(cond, msg?)` | Raises `AssertionError` on failure |
| `raise(msg)` | Raise an error |
| `time()` | Current Unix time in seconds |

Scripts read command-line arguments from the global `argv` (for `dc run file.dc a b`, that is `["a", "b"]`).

## Imports

```dc
import math;                # standard library
import dream;
import "lib/greet.dc";     # path relative to the current file; the module name is the file name (greet)
```

Top-level names in a user module that do not start with `_` become the module's members. A module runs once and is cached. A circular import raises `ImportError`.

## Standard library

`math`: `pi`, `e`, `sqrt`, `floor`, `ceil`, `abs`, `pow`, `min`, `max`

`dream`: `sleep(hours)` (under 7 hours raises `TimeoutError`), `wake()` (prints `Segmentation fault (dream dumped)`), `dump`

## Errors

Every error carries a line number and looks like `file:line: ErrorType: message`:

```
main.dc:2: NameError: name 'zzz' is not defined
Segmentation fault (dream dumped)
```

Common error types: `SyntaxError`, `NameError`, `TypeError`, `IndexError`, `KeyError`, `ZeroDivisionError`, `AttributeError`, `ImportError`, `AssertionError`, `RecursionError`.

# 16 dc eval: A Dream in One Line

`dc eval` is a command for running a single line of code. It takes a string as code, executes it immediately in the interpreter, and writes the result to standard output. It suits quickly checking an expression, or trying out an idea without creating a file.

Usage is:

```
$ dc eval 'print(1 + 2)'
3
```

Because `eval` needs no file, it is well suited to small explorations in the terminal. For example, if you want to know how a built-in function behaves, you can run `dc eval 'print(range(3))'` directly, without creating a script first.

Unlike `dc run`, the error output of `eval` is formatted slightly differently. It prints `line <number>: <error message>` and exits with code 4, but it does not print a segfault. This is because `eval` leans toward daytime exploration. Its user is usually sitting in front of the terminal and does not need the ritual output of waking up. This difference also shows that `eval` is not part of the full dream flow. It is a helper tool.

The code given to `eval` is a complete fragment of a program, and it can contain several statements. For example:

```
$ dc eval 'let x = 2; print(x * 3);'
6
```

Note that code in `eval` runs in the same environment as code in a file. It can access all the built-in functions and import standard library modules. However, it cannot access variables defined in earlier `eval` calls, because every call creates a new interpreter instance.

In practice, `eval` is often used to check the behavior of the standard library. For example, to confirm how `math.floor` handles negative numbers, you can run `dc eval 'import math; print(math.floor(-2.5));'` directly. This immediate feedback helps build an intuition for the details of the language.

From a design point of view, `eval` exists to lower the threshold for exploration. Dream coding emphasizes interpretation after waking, but daytime exploration is just as important. `eval` lets developers quickly test their assumptions during the day, and then write the confirmed ideas into a dream script and hand them over to sleep to work out.

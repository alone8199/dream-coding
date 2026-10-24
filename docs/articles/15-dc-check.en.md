# 15 dc check: Look at the Dream, Don't Run It

[中文](./15-dc-check.md) | [English](./15-dc-check.en.md)

`dc check` is a command that performs only a syntax check. It reads a `.dc` file, tries to parse its syntax, and reports the result, but it does not execute any code. Its job is to catch obvious typing mistakes before the script runs.

Usage is very simple:

```
$ dc check dreams/hello.dc
dreams/hello.dc: ok
```

If the syntax is correct, it prints the file path followed by `ok`, and exits with code 0. If the syntax is wrong, it prints the file name, the line number and the error message, and exits with code 4:

```
$ dc check bad.dc
bad.dc:2: SyntaxError: unexpected ')'
```

This check cares only about syntax, not semantics. It can catch structural problems such as mismatched brackets, unclosed strings, and keywords used in the wrong place. It will not catch undefined variables, a wrong number of function arguments, or division by zero. Those only show up at runtime.

`dc check` exists because daytime writing easily contains small mistakes. An extra bracket, or a string that was never closed, can waste an entire night of sleep. Running `dc check` once before going to bed is a cheap insurance policy.

In CI, `dc check` is also used to check every example under the `dreams/` directory. The pipeline runs the check on every `.dc` file, and any syntax error fails the build. This keeps the example files parseable at all times, even if their semantics have not been fully verified.

Note that passing `dc check` does not mean the script will run correctly. It only shows that the structure of the script is legal. A syntactically correct script can still go wrong at runtime, for example by referencing a module that does not exist, or by setting off infinite recursion inside a loop.

In terms of implementation, `dc check` only calls the parser. It does not create an interpreter, and it does not execute any statement. This makes it fast and free of side effects. It is one of the few verification tools in dream coding that belong purely to the daytime.

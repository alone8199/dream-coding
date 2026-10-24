# 32 The Standard Library: math and dream

[中文](./32-standard-library.md) | [English](./32-standard-library.en.md)

The standard library is a set of modules that ship with `dc`. They are loaded with `import` and provide common mathematical functions and dream-related features. The goal of the standard library is to let common scripts run without depending on external code, while staying small enough to understand and maintain.

The `math` module provides basic mathematical tools. It contains two constants: `pi`, the circle constant, and `e`, the natural constant. It contains `sqrt` for square roots, which raises a `ValueError` when given a negative number. It contains `floor` and `ceil`, which round down and up. It contains `abs` for absolute values and `pow` for exponentiation. `min` and `max` compare a group of numbers, and they accept either several arguments directly or a single list.

The following example shows common uses of `math`:

```dc
import math;
print(math.sqrt(16), math.max(1, 9, 3), math.min([4, 2]));
print(math.floor(2.7), math.abs(-3));
```

The `dream` module corresponds directly to the flow of dream coding. Its `sleep` function takes a number of hours, and it behaves the same way as the `dc sleep` command: the default is eight hours, and less than seven hours raises `TimeoutError`. On success, it prints a message that it has entered the REM stage and returns a string.

The `wake` function prints the segfault message. Its effect is the same as the `dc wake` command: both print `Segmentation fault (dream dumped)`. Placing it at the end of a script lets the script explicitly announce its waking.

`dump` is a constant that holds the text of the segfault message. A script can use it for comparison, or print it elsewhere when needed.

The `dream` module allows a dream script to express part of the flow. For example, a script can first call `dream.sleep(8)`, then perform some calculations, and finally call `dream.wake()`. In this way, the structure of the whole script corresponds to the flow of dream coding.

Other standard library features, such as `len`, `range` and `print`, are provided as global functions and do not need to be imported. They are used more frequently, so they live directly in the global environment.

It is worth noting that the standard library's functions are all native functions, implemented in C++. This means their behavior is fixed and does not change from script to script. To change how a function behaves, you can only modify the runtime code, or register a module with the same name to override it.

In short, the standard library strikes a balance between features and size. It provides enough tools to support common dream scripts, while keeping a simple structure that lets developers quickly understand what each function does.

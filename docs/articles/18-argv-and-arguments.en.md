# 18 Command-Line Arguments and argv

[中文](./18-argv-and-arguments.md) | [English](./18-argv-and-arguments.en.md)

The same dream script often needs to receive different inputs. For example, a script that computes the Fibonacci sequence may want a different number of terms each time it runs. `dc` supports this need through the global variable `argv`.

`argv` is available only in `dc run`. When you execute `dc run script.dc a b c`, every argument after the file path, namely `a`, `b` and `c`, is collected into a list and passed to the script as the value of `argv`. The script can read it like any ordinary list:

```dc
print(len(argv));
for arg in argv {
    print(arg);
}
```

Running `dc run script.dc a b c` prints `3`, then prints `a`, `b` and `c` in turn. Note that the list contains neither `dc` nor `run`, and it does not contain the script's path either. `argv` holds only what the caller actually wants to pass to the script.

Every element of `argv` is a string. If you need numbers, convert them in the script with `num` or `int`. For example, `int(argv[0])` converts the first argument to an integer. If the conversion fails, `num` raises a `ValueError`, which is consistent with the ordinary rules for string handling.

If no extra arguments are passed, `argv` is an empty list. The script should account for this case, for example by checking `len(argv)` first before deciding how to use it. This defensive style lets the script give a meaningful hint when arguments are missing, instead of failing outright.

`dc eval` does not support `argv`. Its code is a string passed in directly, and it has no concept of a file path or extra arguments. If you need arguments in a line of code, you can write them directly into the string.

From a design point of view, `argv` keeps dream scripts general. The same script can be used in different situations without changing its code for each one. This fits the spirit of dream coding: the content of a dream may change with its input, while the structure of the dream stays the same.

Finally, a reminder: arguments are external input, so be wary of them before you use them. Do not assume arguments are always valid, and do not use them directly in sensitive operations. Dream scripts run in the subconscious, but their input still comes from reality.

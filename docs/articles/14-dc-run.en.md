# 14 dc run: Executing a Dream Script

[中文](./14-dc-run.md) | [English](./14-dc-run.en.md)

`dc run` is the command for executing a dream script. It takes the path to a `.dc` file, compiles it in the subconscious, and runs it in non-existence. From the user's point of view, its behavior can be summed up in three steps: read the file, execute the script, and decide the output and exit code based on the result.

The simplest usage is:

```
$ dc run dreams/fib.dc
fib(0..9): 0, 1, 1, 2, 3, 5, 8, 13, 21, 34
counter after 3 calls: 3
```

All `print` output from the script is shown in order. When the script ends normally, `dc run` prints no extra segfault line, and its exit code is 0. This means the script itself ran successfully, but it does not mean the dream is complete. A complete dream still needs to be declared over by waking up, through `dc wake` or the `dream.wake()` call in the script.

`dc run` checks the file extension. Only files ending in `.dc` are executed. Any other extension produces an error directly and exits with code 2. The purpose of this rule is to prevent plain text or source files from other languages from being mistaken for dreams. The comparison of the extension is case-insensitive.

When the file does not exist, `dc run` prints `FileNotFound: no dream at '<path>'` and exits with code 3. Note that this error happens before the script starts, so no segfault output appears.

When the script fails during execution, things are different. `dc run` prints the name of the file, the line number and the error message, then prints the segfault line, and finally exits with code 4. For example:

```
$ dc run bad.dc
bad.dc:2: NameError: name 'zzz' is not defined
Segmentation fault (dream dumped)
```

The combination of these two lines of output is deliberate. The first line tells you where the problem is, and the second keeps the ritual of dream coding. Together they make the error both locatable and in keeping with the ceremony of waking up.

`dc run` also accepts extra command-line arguments. Every argument after the file path is collected into the script's global variable `argv`. For example, `dc run script.dc a b` makes the script see `["a", "b"]`. This lets the same dream script receive different inputs without copying it for each case.

In terms of implementation, `dc run` creates a new interpreter instance, registers the `argv` variable, and then calls `run_file` to execute the file. It keeps no state, so every run is independent. This means the results of several runs can be compared directly.

# 13 Overview of the dc Commands

[中文](./13-dc-command-overview.md) | [English](./13-dc-command-overview.en.md)

`dc` is the command-line tool for dream coding. It splits the whole process into several independent commands, each corresponding to one stage of the process. Understanding how the commands divide the work is the foundation for using `dc`.

There are seven commands at present. `sleep` marks falling asleep and checks the length of sleep. `wake` marks waking up and prints the segfault. `run` executes a dream script. `check` only checks the syntax of a script. `eval` executes one line of code. `version` and `help` show the version and the help message.

Following the order of the process, they are used roughly like this: during the day, use `check` to check your script; before sleeping, use `sleep` to mark the sleep; after waking, use `wake` to confirm you are awake; when needed, use `run` or `eval` to do some verification during the day. Note that `run` and `eval` are not used only during the day. They can also be used to check whether last night's script still runs.

The design of the arguments follows a simple principle: any argument that can be omitted is omitted, and any argument that can have a default value is given one. For example, `sleep` with no argument defaults to eight hours of sleep, and `run` needs only a file path, with the remaining arguments passed to the script's `argv`. This design keeps common operations short.

Every command reports its result through its exit code. Success returns 0, a usage error returns 2, insufficient sleep returns 1, a missing file returns 3, and an error during script execution returns 4. This convention lets `dc` be called from scripts and from CI. For example, the smoke test in CI judges whether the minimum sleep rule is enforced correctly by checking the exit code.

The output style is consistent too. Normal information goes to standard output, and error messages go to standard error. The `print` output of a script belongs to the script alone, and `dc` does not slip any extra information into it. This allows the output of `dc run` to be piped directly into other tools, for example `dc run dreams/fib.dc | grep fib`.

`dc` has no interactive REPL and no configuration file. Its behavior is determined entirely by the command-line arguments and the script itself. This stateless design makes results reproducible: the same command and the same script should produce the same output on any machine.

Finally, the names of the commands are themselves part of the dream metaphor. `sleep` and `wake` are the two most ritualistic commands in the process, and their names come directly from sleeping and waking up. The other commands are more practical. They exist to keep daytime work and nighttime dreams connected.

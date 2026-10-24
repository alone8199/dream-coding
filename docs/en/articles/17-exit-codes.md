# 17 The Exit Code Convention

`dc` uses exit codes to tell its caller what happened. The convention is small, but it is the foundation that lets scripts and continuous integration use `dc` reliably. Understanding what each exit code means avoids misjudging results in automated workflows.

Exit code 0 means success. A `dc sleep` of acceptable length, a normal `dc wake` output, a `dc run` script that ends normally, a `dc check` with correct syntax, and a successful `dc eval` all return 0. Note that a successful `dc run` prints no segfault, so you cannot judge whether a script succeeded by whether a segfault appeared. You have to look at the exit code.

Exit code 1 means the sleep was too short. Only `dc sleep` returns this code. Its output is `TimeoutError: sleep too short`, meaning the duration passed in is below the seven-hour minimum. The smoke test in CI checks this return value specifically, to make sure the minimum rule has not been broken.

Exit code 2 means a usage error. A nonexistent command name, a missing required argument, a sleep duration that is not a valid number, or a `run` file whose extension is not `.dc` will all produce this return value. It indicates a problem with how the command was called, not a problem with the script itself.

Exit code 3 means a file error. `dc run` or `dc check` returns this code when the specified file cannot be found. It happens before the script starts, so it produces no execution result.

Exit code 4 means a program error. Whether it is a syntax error or a runtime error, this code is returned. In this case `dc run` also prints a segfault, while `dc check` and `dc eval` print only the error message. This is the most common failure return value, because it covers every problem in the script itself.

This convention involves a design trade-off: it merges syntax errors and runtime errors into the same code. The benefit is simplicity. The caller only needs to decide whether "the script went wrong," without distinguishing the stage at which it failed. The cost is that the caller cannot tell from the exit code alone where the error happened, and must read the error message as well.

In automated scripts, the recommended approach is to check the exit code first, then decide what to do. For example: if the code is 0, continue to the next step; if it is 4, record the error and stop; if it is 1 or 2, the command was called incorrectly and needs to be fixed. This layered handling is more valuable than a simple "success or failure" check.

Exit codes are also an interface between dream coding and reality. They report the result calmly, without any ritual description attached. The segfault output is for people, and the exit code is for machines. The two complement each other, so that `dc` can be understood by humans and integrated by programs.

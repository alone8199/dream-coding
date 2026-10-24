# 36 FAQ and Troubleshooting

This article collects the problems most often encountered when using `dc` and dream coding, and offers ways to investigate them. Most problems have clear causes, and if you check in the right order, you can usually find the answer quickly.

The first category concerns sleep. If `dc sleep` reports `TimeoutError: sleep too short`, the duration passed is less than seven hours. The solution is to increase the duration, or to accept the fact that a shorter sleep will not produce a complete dream. Note that this check applies only to the command itself; it cannot tell how long you actually slept.

The second category is script runtime errors. The first line of output from `dc run` contains the file name and the line number, which is the starting point for troubleshooting. Open the corresponding file, go to that line, and check the variable names, operators and function calls on it. If the error is `NameError`, the variable is usually not defined with `let`, or it is misspelled. If the error is `TypeError`, the type of an operand for some operation usually does not meet its requirement.

The third category is syntax errors. `dc check` finds most syntax problems before running the script. Its output format is the file name, the line number and the error message, where the error message begins with `SyntaxError`. When you encounter this kind of error, check the brackets, quotes and keywords near the reported line. Note that the reported line is where the parser found the problem; the actual mistake may be earlier, such as a forgotten closing brace.

The fourth category is recursion errors. When `RecursionError: stack overflow in subconscious` appears, first check the termination condition of the function. Make sure each recursive call makes the problem simpler, and that there is a clear base case. If deeper recursion is really needed, consider rewriting it as a loop.

The fifth category is import errors. `ImportError: no module named` usually means a wrong path. Note that file paths are relative to the current file, not to the directory where the command was run. If `circular import` appears, check the dependencies between modules, and extract the shared parts into a third module.

The sixth category concerns files. `FileNotFound` means the specified file cannot be found, and in this case the exit code is 3; confirm that the path is correct. `dc run` requires the file suffix to be `.dc`, otherwise it exits with code 2, which is also easy to overlook.

The seventh category is confusion about output. Many people wonder why a script that ran successfully did not print the segfault message. The segfault message is printed when you wake up, and also when `dc run` ends with a runtime error; a script that runs successfully does not print it. If you want to announce that you have woken up, call `dream.wake()` at the end of the script, or run `dc wake`.

Finally, one issue deserves a separate reminder: if the result of a dream is completely different from what you expected, do not rush to change the code. First return to the steps of interpreting the dream, record what you remember, and then compare it with the script. Most of the time, the problem lies in an unclear daytime description, not in an error in the code.

In short, the key to troubleshooting is to check in order: first the exit code, then the error message and line number, then the corresponding code, and only then deeper causes. This habit applies both to `dc` and to the whole process of dream coding.

# 34 Testing Strategy: Unit Tests and End-to-End Tests

[中文](./34-testing-strategy.md) | [English](./34-testing-strategy.en.md)

Testing is the foundation for keeping `dc` behaving correctly. Because the core of dream coding happens during sleep, testing plays a special role here: it is the only means of verifying behavior with certainty during the day. For this reason, `dc`'s testing strategy has several layers, and each layer targets different risks.

The lowest layer is the unit tests for the runner, located in `runtime/tests/runner_test.cpp`. They test the command-line behavior: whether `help` and `version` print the correct content, whether the duration check of `sleep` takes effect, whether `wake` prints the segfault message, whether a missing file or wrong suffix returns the correct exit code, and whether unknown commands are rejected. These tests do not concern script execution; they only care about the contract of the commands themselves.

The second layer is the interpreter tests, located in `runtime/tests/interp_test.cpp`. They create an interpreter directly, execute code fragments, then check the output and errors. The tests cover arithmetic, strings, variable scope, control flow, functions, closures, lists, dictionaries, logical short-circuiting, error line numbers, syntax errors, the recursion limit, built-in functions, the standard library, native extensions and file modules. Each test is a small script fragment, and when one fails, it prints the expected and actual values, making the problem easier to locate.

File module tests are a special case. They create module files in the system's temporary directory, then run the entry file through `run_file` and check that the imports are correct. The files are cleaned up after the test. This approach covers real file operations without leaving clutter in the repository.

The third layer is end-to-end smoke tests, located in the continuous integration configuration. They do not call internal interfaces directly; instead they run the installed `dc` from the command line. They check the version output, the execution of the entry dream, the syntax and output of the example scripts, and the exit codes for runtime errors. The value of this layer is that it verifies how the whole program behaves in a real environment, not just the correctness of individual modules.

The test design follows one important principle: every test should be quick to understand. A test's name describes the behavior it checks, and its assertion clearly states the expected result. When a test fails after a code change, the developer should be able to find the problem directly from the test name and assertion.

Another principle is that tests for error messages are as important as tests for normal output. Much of `dc`'s value lies in its error reports, such as messages with line numbers and the output of the segfault message. Therefore, the tests contain many assertions that specifically check error content, ensuring that their format and position are not accidentally changed during refactoring.

Tests also serve as documentation. Readers can learn how a feature should be used, and how it behaves at boundary conditions, by reading the tests. For example, that `dc sleep 4` should fail is stated by an explicit assertion in the tests.

In short, the testing strategy of `dc` runs from small to large, from unit to end-to-end, covering every layer from the command line to the interpreter. Its goal is to let every change be verified quickly, and to keep the ritual of dream coding from being undermined by accidental implementation errors.

# Dream Coding Technical Articles

Technical articles on the dream coding technique and the dc interpreter. Each article can be read on its own.

## Part 1: Concepts and Principles

- [01 What Is Dream Coding](./01-what-is-dream-coding.md)
- [02 From Vibe Coding to Dream Coding](./02-from-vibe-to-dream.md)
- [03 Why Sleep as a Development Environment](./03-why-sleep.md)
- [04 The Three Principles of Dream Coding](./04-three-principles.md)
- [05 Metaphor Mapping of Dreams: Compile, Run, Debug, Commit, Deploy](./05-metaphor-mapping.md)

## Part 2: Workflow

- [06 Daytime: How to Write a Dream Script](./06-daytime-writing.md)
- [07 Falling Asleep: dc sleep and the Seven-Hour Minimum](./07-dc-sleep.md)
- [08 Waking Up: dc wake and What segfault Means](./08-dc-wake.md)
- [09 Interpreting Dreams: Recording the Memories That Remain on Waking](./09-interpreting-dreams.md)
- [10 When to Commit: Why Only at Sleep](./10-when-to-commit.md)
- [11 The Art of Deploying: Pretending Nothing Happened on Waking](./11-deploy-and-pretend.md)
- [12 Dreams in Continuous Integration: How Actions Verifies Dreams](./12-ci-verifies-dreams.md)

## Part 3: Command-Line Tools

- [13 dc Command Overview](./13-dc-command-overview.md)
- [14 dc run: Executing a Dream Script](./14-dc-run.md)
- [15 dc check: Looking at the Dream Without Running It](./15-dc-check.md)
- [16 dc eval: One Line of Dream](./16-dc-eval.md)
- [17 Exit Code Conventions](./17-exit-codes.md)
- [18 Command-Line Arguments and argv](./18-argv-and-arguments.md)

## Part 4: Interpreter Internals

- [19 Architecture Overview: From Source Code to the Subconscious](./19-architecture-overview.md)
- [20 Lexer Design](./20-lexer-design.md)
- [21 Recursive Descent Parsing](./21-recursive-descent-parser.md)
- [22 The Single-Node Syntax Tree](./22-ast-single-node.md)
- [23 Value Representation: Variant and Reference Semantics](./23-value-representation.md)
- [24 Scope and the Environment Chain](./24-scope-and-environment.md)
- [25 Functions and Closures](./25-functions-and-closures.md)
- [26 Control Flow: Implementing break, continue, and return](./26-control-flow.md)
- [27 Call Depth Limit and "stack overflow in subconscious"](./27-recursion-limit.md)
- [28 Error Model: DcError with Line Numbers](./28-error-model.md)

## Part 5: Extensions and the Standard Library

- [29 Import Mechanism and Module Cache](./29-import-mechanism.md)
- [30 Registering Native Functions](./30-native-functions.md)
- [31 Registering C++ Modules](./31-native-modules.md)
- [32 Standard Library: math and dream](./32-standard-library.md)

## Part 6: Build, Test, and Release

- [33 Build System: CMake and Cross-Platform Support](./33-build-system.md)
- [34 Testing Strategy: Unit Tests and End-to-End Tests](./34-testing-strategy.md)
- [35 Versioning and the Release Process](./35-versioning-and-release.md)
- [36 FAQ and Troubleshooting](./36-faq-and-troubleshooting.md)

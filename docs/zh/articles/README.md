# 梦境编程技术文章

这里收集了一组专门讲解梦境编程这项技术的文章。它们从概念出发，经过日常工作流程，再深入到 `dc` 的实现细节，最后讨论构建、测试、发布和常见问题。每篇文章大约一千字，可以独立阅读，也可以按顺序阅读。

## 第一部分：概念与原则

- [01 梦境编程是什么](./01-what-is-dream-coding.md)
- [02 从 vibe coding 到 dream coding](./02-from-vibe-to-dream.md)
- [03 为什么选择睡眠作为开发环境](./03-why-sleep.md)
- [04 梦境编程的三条原则](./04-three-principles.md)
- [05 梦境的隐喻映射：编译、运行、调试、提交、部署](./05-metaphor-mapping.md)

## 第二部分：工作流程

- [06 白天：写下梦境脚本的方法](./06-daytime-writing.md)
- [07 入睡：dc sleep 与七小时的下限](./07-dc-sleep.md)
- [08 醒来：dc wake 与 segfault 的含义](./08-dc-wake.md)
- [09 解梦：如何记录醒来后残留的记忆](./09-interpreting-dreams.md)
- [10 提交的时机：为什么只在入睡时 commit](./10-when-to-commit.md)
- [11 部署的艺术：醒来后假装什么都没发生](./11-deploy-and-pretend.md)
- [12 持续集成中的梦：Actions 如何验证梦](./12-ci-verifies-dreams.md)

## 第三部分：命令行工具

- [13 dc 命令总览](./13-dc-command-overview.md)
- [14 dc run：执行一个梦境脚本](./14-dc-run.md)
- [15 dc check：只看梦不跑](./15-dc-check.md)
- [16 dc eval：一行梦](./16-dc-eval.md)
- [17 退出码约定](./17-exit-codes.md)
- [18 命令行参数与 argv](./18-argv-and-arguments.md)

## 第四部分：解释器内部

- [19 整体架构：从源码到潜意识](./19-architecture-overview.md)
- [20 词法分析器的设计](./20-lexer-design.md)
- [21 递归下降语法分析](./21-recursive-descent-parser.md)
- [22 语法树的单一节点结构](./22-ast-single-node.md)
- [23 值的表示：variant 与引用语义](./23-value-representation.md)
- [24 作用域与环境链](./24-scope-and-environment.md)
- [25 函数与闭包](./25-functions-and-closures.md)
- [26 控制流：break、continue 与 return 的实现](./26-control-flow.md)
- [27 调用深度限制与 stack overflow in subconscious](./27-recursion-limit.md)
- [28 错误模型：带行号的 DcError](./28-error-model.md)

## 第五部分：扩展与标准库

- [29 导入机制与模块缓存](./29-import-mechanism.md)
- [30 注册原生函数](./30-native-functions.md)
- [31 注册 C++ 模块](./31-native-modules.md)
- [32 标准库：math 与 dream](./32-standard-library.md)

## 第六部分：构建、测试与发布

- [33 构建系统：CMake 与跨平台](./33-build-system.md)
- [34 测试策略：单元测试与端到端测试](./34-testing-strategy.md)
- [35 版本与发布流程](./35-versioning-and-release.md)
- [36 常见问题与排错](./36-faq-and-troubleshooting.md)

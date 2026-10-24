# 从零学习 Dream Coding

[中文](./tutorial.md) | [English](./tutorial.en.md)

项目地址：<https://github.com/alone8199/dream-coding>

这篇教程带你从零开始学会 `dc` 和 `.dc` 脚本。每个例子都经过实际运行，输出就是你在终端里会看到的内容。

## 1. 准备

你需要一个支持 C++17 的编译器和 CMake。

```bash
git clone https://github.com/alone8199/dream-coding.git
cd dream-coding
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

构建完成后，`build/dc`（Windows 上是 `build/Release/dc.exe`）就是命令行工具。你可以把它加入 `PATH`，也可以直接用完整路径运行。

`dc` 有七个命令：

| 命令 | 作用 |
|---|---|
| `dc run <文件>` | 运行一个 `.dc` 脚本 |
| `dc check <文件>` | 只检查语法，不运行 |
| `dc eval '<代码>'` | 运行一行代码 |
| `dc sleep [小时]` | 记录入睡，默认八小时，不足七小时会报错 |
| `dc wake` | 记录醒来，输出段错误 |
| `dc version` | 显示版本 |
| `dc help` | 显示帮助 |

## 2. 第一个梦

新建 `hello.dc`：

```dc
import dream;

print("hello, dream");
dream.wake();
```

运行：

```bash
dc run hello.dc
```

输出：

```
hello, dream
Segmentation fault (dream dumped)
```

最后一行是正常的。它表示梦已经运行完毕，醒来后你应该去解读梦，而不是去调试它。

注意两点：语句末尾的分号可以省略，但写上更清楚；`import dream;` 是使用 `dream.sleep` 和 `dream.wake` 所必需的。

## 3. 变量

用 `let` 声明变量，用普通赋值修改它：

```dc
let name = "梦";
let count = 1;
count = count + 1;
print("hello,", name, count);
```

输出：`hello, 梦 2`

`print` 会用空格分隔它的参数。

## 4. 基本类型

| 类型 | 例子 | 说明 |
|---|---|---|
| 数字 | `42`、`3.14` | 都是浮点数 |
| 字符串 | `"dc"` | 双引号，支持 `\n`、`\t`、`\r`、`\0`、`\"`、`\\` |
| 布尔 | `true`、`false` | |
| 空值 | `null` | |
| 列表 | `[1, 2, 3]` | |
| 字典 | `{"ann": 30}` | 键是字符串，遍历时按键排序 |

字符串可以用 `+` 拼接，但不同类型不能直接相加，需要先用 `str()` 转换：

```dc
print("ab" + "cd");          # abcd
print("x" + str(1));         # x1
```

## 5. 列表与字典

```dc
let nums = [3, 1, 2];
nums.push(9);
print(len(nums), nums.pop());   # 4 9

let ages = {"ann": 30, "bob": 25};
print(ages.keys());             # ["ann", "bob"]
print(ages.get("bob"));         # 25
print(ages.has("carol"));       # false
```

列表的常用方法有 `push`、`pop`、`reverse`、`join`。字符串的方法有 `upper`、`lower`、`strip`、`split`、`replace`、`startswith`、`endswith`。

## 6. 条件与循环

```dc
let i = 0;
while i < 3 {
    i = i + 1;
    if i == 2 { continue; }
    print("i =", i);
}
```

输出：

```
i = 1
i = 3
```

`for` 用于遍历列表、字符串（按字节）和字典（按键）：

```dc
for n in range(1, 6) {
    print(n);
}
```

`range(a, b)` 返回从 `a` 到 `b-1` 的数字列表，也可以写成 `range(b)` 或 `range(a, b, 步长)`。

`break` 结束最近的循环，`continue` 跳到下一次迭代。逻辑运算用 `and`、`or`、`not`，成员判断用 `in`：

```dc
print(2 in [1, 2, 3]);                  # true
print(true and not false);              # true
```

## 7. 函数

```dc
fn square(x) {
    return x * x;
}
print(square(7));    # 49
```

函数可以递归：

```dc
fn fact(n) {
    if n <= 1 { return 1; }
    return n * fact(n - 1);
}
print(fact(10));     # 3628800
```

函数也可以是值。没有名字的 `fn(...) { ... }` 是匿名函数，它可以捕获外层的变量：

```dc
fn make_counter() {
    let count = 0;
    return fn() {
        count = count + 1;
        return count;
    };
}

let c = make_counter();
c();
print(c());         # 2
```

注意：递归深度上限是 200 层。超过时会报 `RecursionError: stack overflow in subconscious`。

## 8. 模块

`import` 有三种用法：

```dc
import dream;                 # 标准库：梦境模块
import math;                  # 标准库：数学模块
import "helpers.dc";          # 文件模块：加载同目录下的 helpers.dc
```

`math` 的成员有 `pi`、`e`、`sqrt`、`floor`、`ceil`、`abs`、`pow`、`min`、`max`：

```dc
import math;
print(math.sqrt(16), math.max(4, 9, 2));    # 4 9
```

`dream` 模块与梦的流程直接对应，后面会讲到。

文件模块的名字取自文件名（去掉 `.dc`）。例如有 `lib/helpers.dc`：

```dc
import "lib/helpers.dc";
print(helpers.shown);       # 通过 helpers.名字 访问
```

以下划线开头的名字是私有的，模块外访问会报错：`helpers._secret` 会得到 `AttributeError`。

命令行参数通过全局变量 `argv` 读取。`dc run hello.dc a b` 时，`argv` 是 `["a", "b"]`。

## 9. 梦的流程

Dream Coding 的核心流程是：白天写梦，入睡，醒来，解梦。

在命令行里，这对应 `dc sleep` 和 `dc wake`：

```bash
dc sleep
# Entering REM cycle...
# zzz... (8h)

dc wake
# Waking up...
# Segmentation fault (dream dumped)
```

`dc sleep` 默认八小时，少于七小时会报 `TimeoutError: sleep too short`。`dc wake` 输出段错误，这是正常结果。

在脚本里，同样的意思用 `dream` 模块表达：

```dc
import dream;
dream.sleep(8);      # 进入睡眠，少于 7 小时会报错
# ... 梦里的代码 ...
dream.wake();        # 醒来，输出段错误
```

## 10. 错误

运行时出错时，`dc` 会打印文件名、行号和错误信息，然后以退出码 4 结束：

```dc
print(ghost);
```

```
t.dc:1: NameError: name 'ghost' is not defined
Segmentation fault (dream dumped)
```

常见的错误有：

- `SyntaxError`：语法错误，`dc check` 能提前发现。
- `NameError`：使用了未定义的名字。
- `TypeError`：运算数类型不对。
- `ZeroDivisionError`：除以零。
- `ImportError`：找不到模块，或者循环导入。
- `RecursionError`：递归太深。

退出码的完整约定见 [使用说明](./usage.md)。

## 11. 下一步

- 想看完整的语法说明：[脚本参考](./scripts.md)
- 想知道怎么用 C++ 扩展 `dc`：[扩展指南](./extending.md)
- 想了解解释器是怎么实现的：[技术文章](./articles/README.md)（36 篇）
- 想读一些不那么正式的文字：[杂谈](./essays/README.md)
- 想参与开发：[CONTRIBUTING.md](../CONTRIBUTING.md)

VS Code 用户可以安装配套的扩展（`.vsix` 安装包），它提供语法高亮、智能提示、语法诊断，以及 `Ctrl+F5` 直接运行脚本。安装方法见扩展包的说明。

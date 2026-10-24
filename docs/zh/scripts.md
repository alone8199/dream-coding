# 梦境脚本记法参考

`.dc` 梦境脚本的记法接近 Python 和 C，用花括号分块，分号可以省略。`dc` 会把它交给潜意识执行。这份文档只是记录这种记法怎么写，不是语言规范。

## 注释

```dc
# 这是注释
// 这也是注释
```

## 值

| 类型 | 例子 | 说明 |
|---|---|---|
| null | `null` | 空 |
| bool | `true`、`false` | |
| number | `1`、`3.14` | 统一用双精度浮点数 |
| string | `"你好\n"` | 支持 `\n \t \r \0 \" \\` |
| list | `[1, "a", [2]]` | 引用语义，和 Python 一样 |
| dict | `{name: "dc", "k": 1}` | 键是字符串；裸名字会被当作字符串键 |
| function | `fn(x) { return x; }` | 闭包 |

真值规则：`null`、`false`、`0`、空字符串、空列表、空字典为假，其余为真。

## 变量

```dc
let x = 1;       # 在当前作用域定义
x = x + 1;       # 修改已定义的变量（未定义会报 NameError）
```

作用域是块级的：`if`、`while`、`for`、函数体都会开启新作用域。

## 运算符

| 类别 | 运算符 |
|---|---|
| 算术 | `+ - * / %`（`+` 也可以拼接字符串和列表，`*` 可以重复字符串） |
| 比较 | `== != < <= > >=` |
| 成员测试 | `x in container`（列表、字符串子串、字典键） |
| 逻辑 | `and or not`，也可以写 `&& || !`；`and` 和 `or` 短路求值 |
| 一元 | `-x`、`not x` |

`/` 总是得到浮点数结果。除以零会报 `ZeroDivisionError`。

## 控制流

```dc
if x > 0 {
    print("正数");
} else if x == 0 {
    print("零");
} else {
    print("负数");
}

while i < 10 {
    i = i + 1;
    if i == 3 { continue; }
    if i > 6 { break; }
}

for item in [1, 2, 3] {       # 可以遍历列表、字符串（逐字符）、字典（逐键）
    print(item);
}
```

条件不需要括号。

## 函数

```dc
fn add(a, b) {
    return a + b;
}

let square = fn(x) { return x * x; };   # 匿名函数

print(add(1, 2), square(4));
```

函数可以嵌套，内层函数会捕获外层变量：

```dc
fn make_counter() {
    let n = 0;
    return fn() {
        n = n + 1;
        return n;
    };
}
```

函数递归深度上限是 200，超过会报 `RecursionError: stack overflow in subconscious`。

## 容器操作

```dc
let xs = [1, 2, 3];
xs[0];          # 1
xs[-1];         # 3，负数从末尾数
xs[0] = 9;

let d = {a: 1};
d.a;            # 1
d["b"] = 2;
d.b = 3;        # 等价于 d["b"] = 3
```

## 常用方法

列表：`push(v)`、`pop()`、`reverse()`、`join(sep)`

字符串：`upper()`、`lower()`、`strip()`、`split(sep)`、`replace(a, b)`、`startswith(p)`、`endswith(p)`

字典：`keys()`、`values()`、`has(k)`、`get(k, default)`

## 全局函数

| 函数 | 作用 |
|---|---|
| `print(...)` | 打印，参数之间用空格分隔 |
| `len(x)` | 字符串、列表、字典的长度 |
| `str(x)` / `num(x)` / `int(x)` | 类型转换 |
| `type(x)` | 类型名 |
| `range(stop)` / `range(start, stop, step)` | 生成数字列表 |
| `list(x)` | 把可迭代对象转成列表 |
| `assert(cond, msg?)` | 断言失败会报 `AssertionError` |
| `raise(msg)` | 抛出一个错误 |
| `time()` | 当前 Unix 时间（秒） |

脚本通过全局变量 `argv` 读取命令行参数（`dc run file.dc a b` 中的 `a b`）。

## 导入

```dc
import math;               # 标准库模块
import dream;
import "lib/greet.dc";     # 相对于当前文件的路径，模块名取文件名（greet）
```

用户模块中所有不以 `_` 开头的顶层名字会成为模块的成员。模块只执行一次，结果会被缓存。循环导入会报 `ImportError`。

## 标准库

`math`：`pi`、`e`、`sqrt`、`floor`、`ceil`、`abs`、`pow`、`min`、`max`

`dream`：`sleep(hours)`（少于 7 小时报 `TimeoutError`）、`wake()`（打印 `Segmentation fault (dream dumped)`）、`dump`

## 错误

所有错误都带行号，格式为 `文件:行号: 错误类型: 消息`，例如：

```
main.dc:2: NameError: name 'zzz' is not defined
Segmentation fault (dream dumped)
```

常见错误类型：`SyntaxError`、`NameError`、`TypeError`、`IndexError`、`KeyError`、`ZeroDivisionError`、`AttributeError`、`ImportError`、`AssertionError`、`RecursionError`。

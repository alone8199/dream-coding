# 25 Functions and Closures

Functions are the basic unit by which dream scripts organize logic. `dc` supports two ways of writing a function: a named function declaration, which uses `fn` followed by a name, and an anonymous function, which uses `fn` followed by a parameter list directly inside an expression. At runtime both are the same kind of value; the only difference is how they are written.

A function declaration binds its name in the current environment. For example, `fn add(a, b) { return a + b; }` creates a variable named `add` in the current scope, whose value is a function. When called as `add(1, 2)`, the evaluator first looks up `add`, then binds the arguments in order to `a` and `b`, runs the function body, and finally returns the value given by `return`. If the function body has no `return`, the result is nil.

Anonymous functions suit places where a behavior needs to be passed along. For example, `apply(fn(v) { return v * 10; }, 4)` passes a function that multiplies by ten to `apply`, which decides when to call it. An anonymous function needs no name, so it is temporary by nature and is usually used within a single expression.

Closures are an important feature of functions. A function records the environment in which it was defined, and that environment is its closure. Afterward, even if the function is called from somewhere else, it can still access the variables that were visible at its definition. The following example shows this:

```dc
fn make_counter() {
    let n = 0;
    return fn() {
        n = n + 1;
        return n;
    };
}
```

When `make_counter` finishes, its local variable `n` should in principle be destroyed, but the anonymous function it returns still holds a reference to it. Each time the returned function is called, `n` goes up by one, and the result is kept in the same variable. This is the typical use of a closure: encapsulating state inside a function.

The implementation of closures relies on reference counting of environments. A function value holds a shared pointer to the environment in which it was defined, so as long as the function exists, the environment will not be freed. This design means the lifetime of a closure naturally follows that of its function, without the developer having to manage it by hand.

When a function is called, the number of arguments is checked. If the number passed does not match the declaration, the evaluator reports a `TypeError`, naming the function and the expected number of arguments. This check catches argument mistakes at the moment of the call, rather than letting them cause strange behavior inside the function body.

One point needs attention. Function arguments are passed by value, but because lists and dictionaries have reference semantics, a container passed in can still be modified. If you want to avoid side effects, you should copy the container inside the function before operating on it.

From a design perspective, the function model of `dc` tries to stay simple. It has no default arguments, no variadic arguments, and no keyword arguments. These features are useful in large languages, but for dream scripts, plain positional arguments are enough. Simplicity makes a script's intent easier to read, and it also makes interpreting the dream after waking more direct.

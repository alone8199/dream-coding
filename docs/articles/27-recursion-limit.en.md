# 27 Call Depth Limit and "stack overflow in subconscious"

[中文](./27-recursion-limit.md) | [English](./27-recursion-limit.en.md)

Recursion is a powerful programming technique, but it has an obvious risk: if a recursion does not end properly, the call stack keeps growing until it exhausts memory. In `dc`, this situation appears as a special error: `RecursionError: stack overflow in subconscious`.

The source of this error is the call depth limit. The interpreter of `dc` keeps a counter that records how many function calls are currently in progress. Each time a function is called, the counter goes up by one; when the function returns, it goes down by one. When the counter reaches its upper limit, the new call is refused and the recursion error is reported. The current limit is two hundred levels.

The reason for choosing two hundred has to do with the implementation. When a tree-walking interpreter executes one dream function call, it passes through several layers of C++ functions, such as evaluation, function calling, and statement execution. Each layer takes up a certain amount of stack space. If the limit were set too high, on some platforms, such as Windows environments where the default stack is small, the C++ layer could overflow first and crash the program, rather than giving a clear error message. Two hundred is a value that is safe on most platforms.

The purpose of this limit is protective. It turns a recursion failure into an error that can be caught and reported, rather than a crash that is hard to diagnose. When the developer sees this error after waking, they can immediately tell that the problem lies in the recursion, and they can go back to the script to check the termination condition of the recursion.

The phrase "stack overflow in subconscious" in the error message is a deliberate metaphor. In the subconscious, a dream may repeat the same scene endlessly, such as dreaming that you keep climbing the same staircase. In a script, this repetition corresponds to a recursion that never terminates. The error message ties a technical problem to the imagery of dreams, so the developer reading it gets a more direct association.

In practice, running into this error usually means the termination condition of the recursion is wrong. For example, a factorial function that does not handle the case of zero will call itself forever. The fix is to check that the base case is correct, and to make sure every recursive call makes the problem simpler.

It should be noted that this limit applies only to function calls, not to loops. An infinite loop written with `while` or `for` does not trigger the recursion error; it simply keeps running until something outside interrupts it. For this reason, when writing loops, you must also make sure there is a clear exit condition.

If deeper recursion is truly needed, the current approach is to rewrite it as a loop, or to split the problem into several smaller parts. `dc` offers no option to change the limit, in order to keep behavior consistent. In dream coding, a recursion that is too deep is itself a sign that the structure of the dream may be too complicated, and the design deserves to be rethought.

In short, the call depth limit is a simple but effective protective mechanism. It makes recursion errors discoverable in time, and it uses an evocative message to point the developer back to the root of the problem.

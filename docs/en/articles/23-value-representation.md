# 23 Value Representation: Variant and Reference Semantics

Runtime values are the data the evaluator works on. `dc` uses a single `Value` type to represent all values. Internally it is a standard library variant type, which can hold seven different kinds of content: nil, booleans, numbers, strings, lists, dictionaries, and functions. This article explains how these values are represented and what rules govern their behavior.

The benefit of a variant type is that the same variable can hold different kinds of values at different times, without dynamically allocating a base-class object. Each value occupies a fixed amount of memory, and checking its kind takes only one type test. This keeps the cost of evaluation low.

Numbers are uniformly double-precision floating-point values. `dc` does not distinguish integers from decimals; every number is a float. This keeps the arithmetic rules simple, but it also means that very large integers may lose precision. For dream scripts, this is usually not a problem. When printing, a number that is an integer with an absolute value less than one quadrillion is shown as an integer. Otherwise it is shown with up to fifteen significant digits.

Strings are immutable byte sequences. Concatenation, repetition, and slicing all produce new strings. Indexing returns a single byte, not a single character. This matters when working with Chinese text, where each character occupies several bytes.

Lists and dictionaries have reference semantics. This means that when you assign a list to another variable, both variables refer to the same object. Change one, and the other sees the change too. This behavior matches Python, and it lets functions modify a container passed to them with ease, but it also requires the developer to watch for side effects caused by sharing.

The keys of a dictionary are always strings. Numeric keys are converted to strings, and booleans and nil are converted to their corresponding text. Dictionaries are stored internally as an ordered map, so when you iterate over one, the keys come out in lexicographic order, not in insertion order. This needs special attention when writing scripts that depend on order.

Functions are also values. They can be assigned to variables, passed as arguments, and returned from functions. Internally, a function value holds a pointer to a callable object, which may be a user-defined function or a native function implemented in C++. When calling, the evaluator does not need to know where it came from.

The rules for truth follow a common convention: nil, false, zero, the empty string, the empty list, and the empty dictionary are false; everything else is true. This rule lets conditions use containers directly. For example, `if xs { ... }` runs only when the list is not empty.

Equality needs special explanation. Basic types compare by content, so two identical strings are equal. But lists, dictionaries, and functions compare by reference, so only the very same object is equal. As a result, `[1] == [1]` evaluates to false, because they are two different lists.

In short, the design of `Value` strikes a balance between simplicity and expressive power. Its rules are few, but they are enough to support most of what dream scripts need, and they let the developer predict the result of every operation clearly.

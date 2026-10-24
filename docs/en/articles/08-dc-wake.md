# Waking Up: dc wake and What the Segfault Means

`dc wake` is the waking command of dream coding. Running it prints two lines:

```
$ dc wake
Waking up...
Segmentation fault (dream dumped)
```

The first line indicates that the waking process has begun. The second line is the most important one in the whole process: `Segmentation fault (dream dumped)`. Literally, it means a segmentation fault, and the dream was dumped. In dream coding, this line is not a sign of failure but a sign that the process is complete. It means the subconscious has executed the code and the results have been produced, though those results will not stay in memory in a readable form.

Why choose the form of a segfault? A segfault is one of the most frustrating errors in traditional development: the program suddenly crashes without giving a clear reason. Dream coding borrows this image and turns it into a ritual result. Everyone who wakes sees the same line, which reminds you that what you face after waking is not a working program, but a dream that needs to be interpreted.

`dc wake` itself does not check the content of the dream. It does not read any script, and it does not judge whether the execution succeeded. It always prints the same two lines, and this is by design. The fixed output makes the ritual of waking predictable and spares the developer from facing complex information right after waking.

At the script level, the same output can also be produced by a dream script on purpose. The `dream` module in the standard library provides a `wake()` function, and calling it prints the same segfault message. This means a dream script can announce its own waking explicitly, in code.

It should be noted that the correct response after waking is to accept this output, not to look for errors right away. If you run `dc run` after waking and get an error, the error came from the script itself, not from the dream. When `dc run` fails, it also prints a segfault, but the message is preceded by the file name and line number, so the two can be told apart.

There is also an implicit step in waking: dream interpretation. For some time after waking, the developer should recall the impressions the dream left behind and record them. This step is discussed in detail in the next article.

In short, the value of `dc wake` is that it marks the moment of waking and hands the results back to the developer through a fixed ritual. Its output is simple, fixed, and symbolic, and these very traits make the waking process something that can be practiced repeatedly.

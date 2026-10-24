# Daytime: How to Write a Dream Script

The first step of dream coding happens during the day. Its goal is not to write perfect code, but to state intent clearly, giving the subconscious enough clues to work with at night. Therefore, daytime writing needs to follow some habits that differ from ordinary development.

First, a dream script should be short. A dream script usually does only one thing: it defines an entry point, describes a set of key steps, and ends. Scripts that are too long make it hard for the subconscious to grasp the point, and hard for you to recall the details after waking. The `dreams/main.dc` in this project is just over twenty lines, which shows the scale a typical dream script should have.

Second, the script should describe its intent with clear names. Function names, variable names, and module names are all clues for the subconscious. Names like `compile`, `run`, and `commit`, for example, let you quickly understand each step's purpose after waking. Conversely, overly abstract names like `x` or `handle` make the dream lose its direction.

Third, the script can keep some deliberately blank spots. Dream coding allows the developer to write "not known yet" and let the subconscious fill it in. This blankness is not laziness but a conscious design. It admits that daytime cognition is limited and hands the unknown parts over to the night.

Fourth, after writing, run `dc check` for a syntax check. This is the only verification step during the day. It ensures the script can at least be parsed, so you do not waste a whole night's sleep because a bracket was never closed. Note that `dc check` only checks syntax, not semantics, and it does not run the script.

Fifth, after writing, actively close the editor. This is both a symbolic action and a practical constraint. Closing the editor marks the end of the day's work and means you stop changing the script. Many people cannot resist taking one more look at this point, but dream coding asks for restraint: once a script is handed over to sleep, it should not be revised repeatedly during the day.

One point of daytime writing is often overlooked: record your expectations. Before falling asleep, you can write a sentence or two beside the script saying what you hope to see when you wake. These words are not executed, but they will serve as a reference when you interpret the results after waking.

In short, the daytime task is to prepare the starting point of the dream. Keep the script short, make the names clear, make the blanks meaningful, do the check, and then let go.

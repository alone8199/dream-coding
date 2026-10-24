# 01 What Is Dream Coding

[中文](./01-what-is-dream-coding.md) | [English](./01-what-is-dream-coding.en.md)

Dream coding is a technique for arranging development work to be done while you sleep. Its basic assumption is simple: the daytime person proposes ideas, the subconscious at night compiles and runs them, and on waking hands back the results. These results usually cannot be used directly, but they reveal where things went wrong in a particular way.

The core of this technique is not a particular language or framework, but a process. The process has three fixed roles: the daytime developer, the nighttime compiler (that is, the subconscious), and the interpreter who reads the results after waking. The developer writes down intent during the day, hands it to the subconscious after falling asleep, and checks the output after waking. The whole process needs no keyboard and no staring at a screen.

Why do it this way? There are three reasons. First, sleep is a stretch of time free from outside interference, suited to work that requires long chains of reasoning. Second, the first thing you see on waking is the result, which forces you to let go of obsession with details and attend to the overall direction. Third, it provides a ritual that can be practiced repeatedly, helping developers keep their rhythm even when they are tired.

Dream coding has one distinctive feature: its output is always `Segmentation fault (dream dumped)`. This line is not a sign of failure but a sign that the process is complete. It means the subconscious has executed the code, but did not keep the results in memory.

This project uses the `dc` command-line tool to support this process. `dc sleep` enters the sleep phase, `dc wake` completes the waking phase, and `dc run` hands a dream script to the subconscious for execution. The specific behavior of these commands is explained one by one in the later chapters.

It must be emphasized that dream coding is not meant to replace real development. It is a reorganization of the development process: it separates the formation of ideas from the verification of results and inserts a stretch of sleep between the two. For problems that need repeated deliberation, this separation is often more effective than acting immediately.

Learning dream coding requires no special equipment or environment. All you need is a bed, a long enough sleep, and the attitude to accept the result "it didn't run." The following articles expand on the details of this technique at the level of process, tooling, and implementation.

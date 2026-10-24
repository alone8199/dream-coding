# 03 Why Choose Sleep as the Development Environment

[中文](./03-why-sleep.md) | [English](./03-why-sleep.en.md)

Among all the time slots in which development work could be scheduled, sleep looks like the least sensible. A sleeping person cannot read documentation, check logs, or talk to colleagues. But exactly these limitations make sleep a special environment: it isolates external input and keeps only internal deduction.

From the perspective of cognitive science, the brain does not stop working during sleep. Researchers broadly believe that during sleep the brain organizes the information acquired during the day, moves short-term memories into long-term memory, and builds new connections between different concepts. The REM stage is especially active, and dreams usually occur during it. Dream coding borrows the characteristics of this stage: it hands daytime ideas to the brain and lets it recombine them without outside interference.

Such recombination is often leaping. In dreams, two unrelated concepts may suddenly connect, and a complex problem may shrink into a single image. For code, this leaping is sometimes harmful, such as forcibly stitching together two incompatible interfaces. But sometimes it brings new ideas, such as discovering that a feature that would have needed a lot of code can be done with a very simple structure.

Sleep also has one important property: it has a duration. `dc sleep` defaults to eight hours and requires at least seven. This floor was not set arbitrarily; it exists to ensure the brain completes one full sleep cycle. Under seven hours, the dream gets cut off and the results tend to lack coherence. So dream coding has a hard requirement on time.

Sleep as a development environment also has limits. It cannot guarantee the correctness of the results, nor can it control the content of the dream. This means the output of dream coding must be treated with caution, especially in scenarios involving real data or real users. Treating sleep as a development environment is about gaining a different perspective, not about replacing testing and review.

In practice, the greatest benefit of the sleep environment is that it reduces interference from immediate feedback. Daytime developers are easily led along by error messages and fall into local patching. The sleep environment forces developers to think about the whole: what result is wanted, which parts are essential, and which parts can be left out.

Choosing sleep is ultimately choosing a rhythm. It divides the work between day and night, leaving the parts that need quick reactions to the day and handing the parts that need to settle to the night. This division is itself the most important design of dream coding.

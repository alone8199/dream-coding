# 10 When to Commit: Why You Only Commit at Bedtime

[中文](./10-when-to-commit.md) | [English](./10-when-to-commit.en.md)

A commit in version control is a deterministic act. It freezes the state of the code at one moment, writes it into history, and makes it available for others to review and roll back to. In traditional development, commits usually happen after a feature is finished. Dream coding asks that commits happen only at bedtime, or after waking up while you still remember the dream. To understand the reason for this rule, we first need to see what a commit really is.

A commit is a promise. Every commit says: this code is what I believe to be correct, and I am willing to take responsibility for it. This promise makes sense during the day, because a daytime developer can verify the results and has the means to revise them. In dream coding, however, the code is worked out during sleep, and its correctness has not yet been tested. Promising too early fills the repository's history with unverified content.

For this reason, dream coding places the commit at bedtime. This has two meanings. First, going to sleep marks the end of the day's work. By then the developer has made every judgment they can make that day, which makes it a good moment to summarize. Second, what gets committed should be the script written during the day, not a version edited after waking. This keeps the committed content consistent with the input of the dream.

If you still remember the dream after waking and think some part of it is worth keeping, you may make a real commit after waking. This commit should state in its commit message that it came from a dream. This project's `CONTRIBUTING.md` defines the type prefixes. For example, `dream:` marks a new dream, and `nightmare:` marks the fixing of a nightmare, which means fixing a bug.

Forced commits are one of the most common violations of dream coding. Many developers, after waking, cannot resist changing the code and committing it right away, believing this saves their progress. But this makes the history hard to trace. You can no longer tell which changes came from the dream and which came from daytime patching.

The timing of the commit also has a practical benefit: it forces the developer to do a self-check before sleeping. Ask yourself one question: is what I wrote today ready to be handed over to the subconscious? If the answer is no, go back to daytime writing instead of falling asleep with doubts.

In short, a commit is an act that deserves care. Dream coding places it at one specific moment, so that every commit carries a clear-headed promise, and so that the history keeps the boundary between dream and reality.

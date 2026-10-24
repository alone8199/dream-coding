# 11 The Art of Deploying: Wake Up and Pretend Nothing Happened

[中文](./11-deploy-and-pretend.md) | [English](./11-deploy-and-pretend.en.md)

Deploying is the act of handing code over to a real environment. It means users will come into contact with this code, and the system will depend on it to run. Dream coding has a distinctive attitude toward deployment: after waking, pretend nothing happened. The phrase sounds like a joke, but it rests on a serious line of reasoning.

First, running in a dream is not the same as running in reality. Programs in a dream can access unlimited memory, every network request can succeed, and asynchronous callbacks can finish before you wake up. None of these conditions exist in reality. Deploying the results of a dream directly means assuming that reality matches the dream exactly, and that assumption almost never holds.

Second, deployment is an act that carries consequences. It may affect real users' data, change how a service behaves, or cause a failure that needs urgent handling. Dream coding asks the developer to verify against the real environment before deploying. The verification can be a test suite, a staging environment, or human review. Whatever form it takes, it must be independent of the dream itself.

So what does "pretend nothing happened" mean? It means that after waking, you do not change your judgment of reality because of what the dream showed you. You may have seen a perfect solution in a dream, but after waking you still need to verify it the daytime way. A dream is an inspiration, not a proof.

This attitude also protects the developer's state of mind. If you believe the result of a dream as soon as you wake and rush to deploy it, a failure can easily plunge you into anxiety. Conversely, if you treat the dream as a dream and reality as reality, a failure is just part of the process and does not shake your confidence in the method as a whole.

In engineering practice, deployment is usually carried out by continuous integration and release tools. This project's Actions package `dc` into a Release once the build passes. This process has nothing to do with dreams; it relies on daytime tests and review. The dream only provides a starting point for thinking. Real deployment still has to pass the test of reality.

Therefore, deployment in dream coding is a clear-headed act. It asks the developer to stay calm after waking, set the dream's result aside, and decide whether to release using the standards of reality. "Pretend nothing happened" is really a reminder: the dream is the dream, and reality is reality.

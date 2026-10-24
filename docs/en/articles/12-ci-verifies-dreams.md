# 12 Dreams in Continuous Integration: How Actions Verify Dreams

The core of dream coding happens during sleep, but it does not reject continuous integration. On the contrary, continuous integration is an important means of verifying dream scripts during the day. This project's `.github/workflows/build.yml` is one example: it checks dream scripts and the runtime in the same pipeline, making sure that dreams can also be actually executed during the day.

The pipeline has two jobs. The first is the build. On Ubuntu, macOS and Windows, it configures, compiles and runs the unit tests, then installs `dc`. On Ubuntu it also builds an extra Debug version. A Debug build is not optimized, so problems that optimization would otherwise hide are exposed here. The second job is the release, which runs only when a version tag is pushed, or when the workflow is triggered manually with a version number filled in.

The second half of the build job is the smoke test. The smoke test uses the freshly installed `dc` to run a fixed set of commands: it checks the version, runs the entry dream, checks the syntax of the example scripts, verifies the output of the examples, and confirms that a runtime error ends with exit code 4. These checks make sure the basic behavior of `dc` has not been broken, and that the scripts under the `dreams/` directory can always be run.

One rule in the smoke test deserves attention: a sleep shorter than seven hours must fail. This rule is checked explicitly in CI. If someone accidentally changes the lower bound, the pipeline reports an error immediately. This reflects a principle of dream coding: even ritual rules need to be protected by automation.

Continuous integration serves another purpose as well: it records the history of every build. When a build fails on some platform, the developer can read the logs after waking and judge whether the problem came from the dream script or from the runtime itself. This record gives more evidence for interpreting the dream.

Note that continuous integration verifies the state of the daytime, not the content of the dream. CI does not read your dreams, and it does not judge whether a dream is correct. It only guarantees that the code can compile, test and run in a real environment. The meaning of a dream still has to be interpreted by the developer.

In short, continuous integration is the bridge between dream coding and reality. It gives daytime work reliable verification, and it gives nighttime dreams a foundation that can be checked. The two work together to form a complete workflow.

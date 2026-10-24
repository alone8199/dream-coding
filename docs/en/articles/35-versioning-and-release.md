# 35 Versioning and the Release Process

The version number tells users which version of `dc` they are running, and the release process ensures that every version can be downloaded and verified. `dc` has clear conventions for versions and releases, and understanding them helps determine whether a behavior comes from a specific version.

The version number has three parts and follows the basic rules of semantic versioning. The current version is written in two places: the project version in the CMake configuration, and the `kVersion` constant in the runtime code. The two must match, and `dc version` outputs the latter. When releasing a new version, both must be updated; otherwise the printed version would not match the release tag.

The changelog is kept in `CHANGELOG.md`, which follows the Keep a Changelog format. Unreleased changes are first recorded under the `Unreleased` section, grouped by categories such as added, changed and fixed. When releasing, the content is moved into a section with the version number and date. The Chinese and English changelogs should be updated together.

The release process is carried out by continuous integration. The build job compiles and tests `dc` on three platforms, and packages each platform's installed result as a compressed file. The release job is responsible for creating a GitHub Release and attaching these archives to it. The release job runs under only two conditions: when a tag starting with `v` is pushed, or when the workflow is triggered manually with a version number.

Manual triggering is the currently recommended approach for this project. Its benefit is that the tag is created by Actions itself, so there is no need to push a tag from a local machine. To release, run the `build` workflow on the Actions page and enter a version number such as `v0.3.0`; the pipeline will create the tag on the current commit and publish the corresponding Release.

Release artifacts are `tar.gz` archives packaged separately for each platform. The contents of the archive come from the output directory of `cmake --install`, which includes the `bin/dc` executable. Users can download the archive for their platform, extract it, and run it directly.

The version history can be viewed on the Release page. Each Release has automatically generated release notes listing the commits between versions. For versions that need a detailed explanation, a more complete description should be added to `CHANGELOG.md`.

Rollbacks and fixes follow the same process. If a version has a serious problem, a new fix version should be released, rather than modifying the existing tag. This ensures that users can trust what a version number represents, and it keeps the history traceable.

In short, the core of the versioning and release process is reproducibility and verifiability. The version number tells users what they have, continuous integration ensures the artifacts are built from code that has passed tests, and the changelog explains the reason for each change.

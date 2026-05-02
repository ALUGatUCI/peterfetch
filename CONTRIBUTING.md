# Contributing Guidelines

Thank you for wanting to contribute to this project. Before any of your time, please make sure to follow the guidelines below.

## Utility Scripts

The `meta` directory contains several scripts to simply frequently run commands such as building, running, and testing.

They should be run from the project's root directory in the for `./meta/test.sh`.

> Note: you still need to generate the `build` directory manually with `cmake -B build`.

Important scripts:
- `meta/build.sh` (only build)
- `meta/run.sh <any args for peterfetch>` (build and run)
- `meta/test.sh` (build and test)

## Branches

Please do all of your work on a separate branch and not `main`.
As a generic suggestion, consider either `feature/thing-im-working-on` or `dev/my-name`.

## Testing

Tests can be run with `ctest --test-dir build --output-on-failure` (assuming that `build` is your build directory).

Please test your code before opening a pull request. Tests should pass and writing new tests for your changes is expected.

## Coding Conventions

Please follow the conventions that you see within any of the source files you are working on.

> Note: Automated style checks will be introduced soon. After their introduction, please consult them.

## Naming Your Commits

We roughly follow the [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/) standard for commit messages.

The following primary commit message types are used:
- `feat` for new additions to the project
- `fix` for bug fixes
- `maintain` for project maintenance related work

You should probably use one of these.

For example, consider the message for [this](https://github.com/ALUGatUCI/peterfetch/commit/083473add66623c40914456319cefc1f3af0a2b9) commit:
```
feat: collect info from UCI Directory
```

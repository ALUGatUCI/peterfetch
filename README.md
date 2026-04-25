# Peterfetch

A terminal fetch application akin to neofetch but for UCI students.

## Building

### System Dependencies

Ensure your system has the following dependencies:
- A C++ compiler with support for C++20 (see [this table](https://en.cppreference.com/cpp/compiler_support/20))
- CMake
- pkg-config
- libcurl
- curlpp
- [args](https://github.com/Taywee/args)

> Note: For Debian based distributions, you may need to install the `-dev` version of some packages

### Nix/NixOS Systems

If your system has Nix installed, you can use the provided `flake.nix` if you prefer.

Activate the development shell with `nix develop` or if you have `nix-direnv` installed, simply `direnv allow`.
Then, follow the build instructions below.

Alternatively, you can directly build with `nix build` and run with `nix run`, skipping the steps below.

> Note: To view the build logs easier, use `nix build -L --log-format bar-with-logs`.

### Build Commands

You can configure the project with:

```bash
cmake -B build
```

And then compile with:

```bash
cmake --build build
```

This will produce a binary called `peterfetch` in the `build` directory.
You can run it with `./build/peterfetch`.

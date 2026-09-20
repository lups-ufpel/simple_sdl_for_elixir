# SimpleSDL2

A simple Elixir wrapper to create a window using SDL2. This library was designed for raw framebuffer/pixel buffer rendering using Numerical Elixir (Nx) tensors.

## Prerequisites

Because SimpleSDL2 compiles native C++ Erlang NIFs, your system requires a C/C++ compiler, CMake, and the SDL2 development headers.

In Debian-based Linux distributions, you can install these dependencies with the following command:

```bash
sudo apt-get update
sudo apt-get install build-essential erlang-dev cmake libsdl2-dev
```

## Installation

To use SimpleSDL2 in an Elixir project, just add it as a dependency in your `mix.exs` file:

```elixir
defp deps do
  [
    {:simple_sdl2, git: "https://github.com/lups-ufpel/simple_sdl_for_elixir.git"}
  ]
end
```

Then fetch the dependencies and compile:

```bash
mix deps.get
mix compile
```

## Quick Start

Check the [blue window](test/test_sdl2.exs) example to see how to use the library!

## Why not SDL3?

We plan on migrating this library to SDL3 once SDL3 it is widely available on the package managers of most Debian-based Linux distributions. At the moment, the SDL3 library (`libsdl3-dev`) is not available for Debian 12 (Bookworm) and Ubuntu Noble (24.04) users, which also impacts downstream distributions like Linux Mint.

We want to avoid forcing users to compile SDL3 from source, because this process is a bit more complex and inconvenient than simply installing SDL2 from the package manager with a single command.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

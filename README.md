# SimpleSDL2

Simple SDL2 library for Elixir.

## Why not SDL3?

We plan on migrating this library to SDL3 once SDL3 it is widely available on the package managers of most Debian-based Linux distributions. At the moment, the SDL3 library (`libsdl3-dev`) is not available for Debian 12 (Bookworm) and Ubuntu Noble (24.04) users, and therefore, it is also not available for Linux Mint users.

We want to avoid forcing users to compile SDL3 from source, because it is a bit more complex and inconvenient process than installing SDL2 from the package manager in a single command.

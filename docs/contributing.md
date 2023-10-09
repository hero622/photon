# Contributing

## Building

### Windows
- Visual Studio 2019
- MSVC Toolset v142
- Configure SDK version in `src/wormhole.vcxproj`
- Configure paths in `copy.bat`

### Linux
- g++ 10
- g++-8-multilib
- Make 4.1
- Configure paths in `config.mk`

## Pull requests
- Follow the coding style
- Do not stage files that you had to configure

## Coding style
Basic coding style:
- `snake_case` for everything
- `_t` for type aliases

`.clang-format` is included, use an extension

## Coding
Read [Creating mods](creating_mods.md) for basics.
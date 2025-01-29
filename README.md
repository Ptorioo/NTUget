![logo](docs/logo.png)

## Goal

All-in-one API parser & command-line tool for NTU COOL (and possibly other NTU platforms).

This project is still at its early stage, so any contribution is welcomed.

### TODO List

- [x] SSO login support
- [x] Parse unread conversation count
- [x] Parse course info
- [x] Parse dashboard cards
- [x] Parse planner items
- [ ] Parse conversations
- [ ] Support more user actions

## License

NTUget is an open-source project under the [MIT license](/LICENSE).

## Dependencies

NTUget depends on libcurl and libm.

You may also want to install the following dependencies which support the core functions of libcurl:

- ssl
- crypto
- brotli
- libz
- psl

Compiling and installing libcurl from source is strongly encouraged, see more info on its [GitHub repository](https://github.com/curl/curl).

## Build

### Docker

To run the current build of NTUget in a Docker container, use the following commands.

```
docker build -t ntuget .
docker run --rm --name ntuget -it ntuget
```

### CMake

NTUget can be compiled and linked using CMake, you can utilize the current Makefile to simplify the process.

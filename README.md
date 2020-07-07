# UEFI Apps Skeleton without EDK Build System ![Build](https://github.com/retrage/uefi-sample/workflows/Build/badge.svg?branch=master)

uefi-sample is a standalone UEFI application skeleton with less dependencies.
The build system does not require TianoCore/EDK2.
It has header files imported from EDK2 to support
UEFI specific types and functions.
It also includes [mini-printf](https://github.com/mludvig/mini-printf) and
some useful string functions.

## Requirements

* LLVM Clang and LLD
* QEMU

## Building

```
$ make
```

## License

uefi-sample is released under the [BSD 3-Clause License](LICENSE)

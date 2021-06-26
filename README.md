# cpp_util::dynarray

[![Test with GCC, Clang and MSVC](https://github.com/arkantos493/dynarray/actions/workflows/compiler_test.yml/badge.svg)](https://github.com/arkantos493/dynarray/actions/workflows/compiler_test.yml) <br/>
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/4f9e7aed05774d538c7fa6f99f81c26f)](https://www.codacy.com/gh/arkantos493/dynarray/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=arkantos493/dynarray&amp;utm_campaign=Badge_Grade) <br/>
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Implementation of a runtime fixed-size array (loosely based on [N3662](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3662)).

In contrast to `std::array`, the size of a `cpp_util::dynarray` doesn't have to be a compile-time constant. <br>
Additionally, since a `cpp_util::dynarray` doesn't have to grow at runtime, there is no need to save a capacity resulting in less memory used (e.g. 2 byte vs. 3 byte).

The API is inspired by the `std::array` and `std::vector` API without the member functions modifying its size. Three additional functions
are implemented:

- `cpp_util::dynarray::fill(const value_type& value = value_type{})`
- `cpp_util::dynarray::iota(const value_type& value = value_type{})`
- `cpp_util::dynarray::generate(const value_type& value = value_type{})`

## Prerequisites

Any compiler supporting `C++11` should be sufficient (for more information see [Compiler Support](#compiler-support)). <br>
Additionally, at least [CMake](https://cmake.org/) `3.20` is required.

The tests are implemented using [Catch2](https://github.com/catchorg/Catch2/tree/v2.x), which gets shipped as single header file with this repository.

## Building and Running the Examples

Building with GCC, Clang, or MSVC can be done using CMake presets.

```bash
$ git clone git@github.com:arkantos493/dynarray.git
$ cd dynarray
$ cmake --preset [preset] .
$ cmake --build --preset [preset]
$ ./build/dynarray
```

Where `[preset]` is one of `gcc`, `clang`, or `msvc`.

## Building and Running the Tests

To additionally build the tests and run them use:

```bash
$ cmake --preset [preset] -DCPP_UTIL_ENABLE_TESTS .
$ cmake --build --preset [preset]
$ ctest --preset [preset]
```

Tests for all supported `C++` standards starting with `C++11` for the currently used compiler are generated. <br>
For example, when using `g++11` test cases for `C++11`, `C++14`, `C++17`, and `C++20` are generated. <br>
On the other hand, when using `clang-3.9` only tests for `C++11` and `C++14` are generated.

## Compiler Support

The `cpp_util::dynarray` has been tested with the following compilers, all installed using the respective package
manager and GitHub actions (other compilers or compiler versions may also be supported):

| Compiler | Versions                                                                |
| -------- | ----------------------------------------------------------------------- |
| GCC      | 5.5.0, 6.4.0, 7.5.0, 8.4.0, 9.3.0, 10.2.0, 11.1.0                       |
| Clang    | 3.9.1, 4.0.1, 5.0.1, 6.0.1, 7.0.1, 8.0.1, 9.0.1, 10.0.0, 11.0.0, 12.0.1 |
| MSVC     | 19.16.27045.0, 19.25.28614.0, 19.29.30037.0                             |

All tests were run using the following compiler flags:

- GCC: `-Wall -Wextra -Wpedantic -Werror`
  
- Clang: `-Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-zero-as-null-pointer-constant -Wno-unknown-warning-option -Wno-undefined-func-template -Werror`
  - `-Wno-c++98-compat -Wno-c++98-compat-pedantic`: since `C++98` is not supported
  - `-Wno-zero-as-null-pointer-constant`: due to a warning generated by `clang-10.0.0` and `clang-11.0.0` when compiling the Catch2 header
  - `-Wno-unknown-warning-option`: since `clang-3.9.1` and `clang-4.0.1` do not support the flag `-Wno-zero-as-null-pointer-constant`
  - `-Wno-undefined-func-template`: due to a warning generated by `clang-5.0.1`
  - `-Wno-c++1z-extensions`: due to a warning in `clang` when using `[[nodiscard]]` prior to `C++17` (although the corresponding feature test macro is defined)

- MSVC:  `/W4 /EHsc /wd4067 /WX`
  - `/wd4067`: disable warning related to function like feature test macros in `MSVC 14.16`

## Compiler Standard Version

- `C++11`:
  - the minimum required standard version enabling all functions

- `C++14`:
  - the functions `exchange(T&, U&&)` and `make_reverse_iterator(Iter)` are now implemented using the `std::` versions
  - `operator==(const dynarray&, const dynarray&)` is now implemented
    using `std::equal(InputIt1, InputIt1, InputIt2, InputIt2)`

- `C++17`:
  - almost all functions are now marked as `[[nodiscard]]`
  - added a deduction guide for the iterator range constructor `dynarray(ForwardIt, ForwardIt)`

- `C++20`:
  - all functions are now marked as `constexpr`
  - the relational operators are now implemented in terms of the three-way comparison
    operator `operator<=>(const dynarray&, const dynarray&)`
    
The actual features are enabled using the specific features test macros and not the `__cplusplus` macro.
# dynarray

Implementation of a runtime fixed-size array.

In contrast to `std::array`, the size of a `dynarray` doesn't have to be a compile-time constant. Additionally, since
a `dynarray` doesn't have to grow at runtime, it doesn't have to save a `capacity` resulting in less memory used (e.g. 2 byte vs. 3 byte).

The API is inspired by the `std::vector` API without the member functions modifying its size.
Three additional functions are implemented:
- `dynarray::fill()` (same as `std::array::fill()`)
- `dynarray::iota()`
- `dynarray::generate()`

## Usage

Simply copy the `dynarray.hpp` file to your project folder (and change the namespace `util` to something more
sophisticated.)

The implementation supports all standards down to`C++11`. <br>
However, if for example `__cplusplus == 202002L` is fulfilled (`C++20`), `constexpr` versions of all functions are
available.

## Building the examples

```bash
$ mkdir build && cd build
$ cmake ..
$ cmake --build . --target dynarray --config Debug
$ ./dynarray
```
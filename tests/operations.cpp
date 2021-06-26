/**
 * Copyright (C) 2021-06-26 - Marcel Breyer - All Rights Reserved
 * Licensed under the MIT License. See LICENSE.md file in the project root for full license information.
 *
 * Implements tests for the other member functions of the cpp_util::dynarray class.
 */

#include "catch/catch.hpp"
#include "dynarray.hpp"

TEST_CASE("dynarray other member functions", "[operations]") {
  SECTION("swap() member function") {
    cpp_util::dynarray<int> arr1(10, 1);
    cpp_util::dynarray<int> arr2(5, 2);

    REQUIRE(arr1.size() == 10);
    REQUIRE(arr2.size() == 5);

    arr1.swap(arr2);

    REQUIRE(arr1.size() == 5);
    CHECK(std::all_of(arr1.begin(), arr1.end(), [](const int i) { return i == 2; }));
    REQUIRE(arr2.size() == 10);
    CHECK(std::all_of(arr2.begin(), arr2.end(), [](const int i) { return i == 1; }));
  }

  cpp_util::dynarray<int> arr(10);

  SECTION("fill() member function") {
    arr.fill();
    CHECK(std::all_of(arr.begin(), arr.end(), [](const int i) { return i == 0; }));

    arr.fill(42);
    CHECK(std::all_of(arr.begin(), arr.end(), [](const int i) { return i == 42; }));
  }

  SECTION("iota() member function") {
    arr.iota();
    for (std::size_t i = 0; i < 10; ++i) {
      CHECK(arr[i] == static_cast<int>(i));
    }

    arr.iota(42);
    for (std::size_t i = 0; i < 10; ++i) {
      CHECK(arr[i] == static_cast<int>(i) + 42);
    }
  }

  SECTION("generate() member function") {
    auto gen = []() {
      static int n = 0;
      return n++ * 2;
    };
    arr.generate(gen);
    for (std::size_t i = 0; i < 10; ++i) {
      CHECK(arr[i] == static_cast<int>(i) * 2);
    }
  }
}

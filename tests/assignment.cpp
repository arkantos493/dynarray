/**
 * Copyright (C) 2021-06-06 - Marcel Breyer - All Rights Reserved
 * Licensed under the MIT License. See LICENSE.md file in the project root for full license information.
 *
 * Implements tests for the assignment operators of the cpp_util::dynarray class.
 */

#include <algorithm>
#include <utility>

#include "catch/catch.hpp"
#include "dynarray.hpp"

TEST_CASE("dynarray assignment operators", "[assignment]") {
  cpp_util::dynarray<int> arr1 = {42, 42, 42};
  cpp_util::dynarray<int> arr2;

  REQUIRE(arr1.size() == 3);
  REQUIRE(arr2.empty());

  SECTION("copy-assignment operator") {
    arr2 = arr1;

    REQUIRE(arr1.size() == arr2.size());
    CHECK(std::equal(arr1.data(), arr1.data() + 3, arr2.data()));
  }

  SECTION("move-assignment operator") {
    arr2 = std::move(arr1);

    CHECK(arr1.empty());
    CHECK(arr1.data() == nullptr);

    REQUIRE(arr2.size() == 3);
    CHECK(std::all_of(arr2.data(), arr2.data() + 3, [](const int i) { return i == 42; }));
  }

  SECTION("initializer_list-assignment operator") {
    arr2 = {42, 42, 42};

    REQUIRE(arr2.size() == 3);
    CHECK(std::all_of(arr2.data(), arr2.data() + 3, [](const int i) { return i == 42; }));
  }

  SECTION("assign using count and value") {
    arr2.assign(10, 404);

    REQUIRE(arr2.size() == 10);
    CHECK(std::all_of(arr2.data(), arr2.data() + 10, [](const int i) { return i == 404; }));
  }

  SECTION("assign using iterator pair") {
    arr2.assign(arr1.begin(), arr1.end());

    REQUIRE(arr2.size() == 3);
    CHECK(std::all_of(arr2.data(), arr2.data() + 3, [](const int i) { return i == 42; }));
  }

  SECTION("assign using initializer_list") {
    arr2.assign({42, 42, 42});

    REQUIRE(arr2.size() == 3);
    CHECK(std::all_of(arr2.data(), arr2.data() + 3, [](const int i) { return i == 42; }));
  }
}

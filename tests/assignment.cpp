/**
 * Copyright (C) 2021-06-26 - Marcel Breyer - All Rights Reserved
 * Licensed under the MIT License. See LICENSE.md file in the project root for full license information.
 *
 * Implements tests for the assignment operators of the cpp_util::dynarray class.
 */

#include <algorithm>
#include <utility>

#include "catch/catch.hpp"
#include "dynarray.hpp"

TEST_CASE("dynarray assignment operators", "[assignment]") {
  cpp_util::dynarray<int> arr1;
  cpp_util::dynarray<int> arr2 = {42, 42, 42};
  cpp_util::dynarray<int> arr3 = {404, 404, 404};

  REQUIRE(arr1.empty());
  REQUIRE(arr2.size() == 3);
  REQUIRE(arr3.size() == 3);

  SECTION("copy-assignment operator") {
    arr1 = arr2;
    REQUIRE(arr1.size() == arr2.size());
    CHECK(std::equal(arr1.begin(), arr1.end(), arr2.begin()));

    arr1 = arr3;
    REQUIRE(arr1.size() == arr3.size());
    CHECK(std::equal(arr1.begin(), arr1.end(), arr3.begin()));
  }

  SECTION("self copy-assignment") {
    arr2 = arr2;
    REQUIRE(arr2.size() == 3);
    CHECK(std::all_of(arr2.begin(), arr2.end(), [](const int i) { return i == 42; }));
  }

  SECTION("move-assignment operator") {
    arr1 = std::move(arr2);

    CHECK(arr2.empty());
    CHECK(arr2.data() == nullptr);

    REQUIRE(arr1.size() == 3);
    CHECK(std::all_of(arr1.begin(), arr1.end(), [](const int i) { return i == 42; }));
  }

  SECTION("initializer_list-assignment operator") {
    arr1 = {42, 42, 42};
    REQUIRE(arr1.size() == 3);
    CHECK(std::all_of(arr1.begin(), arr1.end(), [](const int i) { return i == 42; }));

    arr1 = {404, 404, 404};
    REQUIRE(arr1.size() == 3);
    CHECK(std::all_of(arr1.begin(), arr1.end(), [](const int i) { return i == 404; }));
  }

  SECTION("assign using count and value") {
    arr1.assign(10, 42);
    REQUIRE(arr1.size() == 10);
    CHECK(std::all_of(arr1.begin(), arr1.end(), [](const int i) { return i == 42; }));

    arr1.assign(10, 404);
    REQUIRE(arr1.size() == 10);
    CHECK(std::all_of(arr1.begin(), arr1.end(), [](const int i) { return i == 404; }));
  }

  SECTION("assign using iterator pair") {
    arr1.assign(arr2.begin(), arr2.end());
    REQUIRE(arr1.size() == 3);
    CHECK(std::all_of(arr1.begin(), arr1.end(), [](const int i) { return i == 42; }));

    arr1.assign(arr3.begin(), arr3.end());
    REQUIRE(arr1.size() == 3);
    CHECK(std::all_of(arr1.begin(), arr1.end(), [](const int i) { return i == 404; }));
  }

  SECTION("assign using initializer_list") {
    arr1.assign({42, 42, 42});
    REQUIRE(arr1.size() == 3);
    CHECK(std::all_of(arr1.begin(), arr1.end(), [](const int i) { return i == 42; }));

    arr1.assign({404, 404, 404});
    REQUIRE(arr1.size() == 3);
    CHECK(std::all_of(arr1.begin(), arr1.end(), [](const int i) { return i == 404; }));
  }
}

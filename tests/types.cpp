/**
 * Copyright (C) 2021-06-06 - Marcel Breyer - All Rights Reserved
 * Licensed under the MIT License. See LICENSE.md file in the project root for full license information.
 *
 * Implements tests for the type aliases of the cpp_util::dynarray class.
 */

#include "dynarray.hpp"
#include "catch/catch.hpp"

#include <type_traits>

TEST_CASE("dynarray member types", "[types]") {
  using array_type = cpp_util::dynarray<int>;

  CHECK(std::is_same<typename array_type::value_type, int>::value);
  CHECK(std::is_same<typename array_type::size_type, std::size_t>::value);
  CHECK(std::is_same<typename array_type::difference_type, std::ptrdiff_t>::value);
  CHECK(std::is_same<typename array_type::reference, int&>::value);
  CHECK(std::is_same<typename array_type::const_reference, const int&>::value);
  CHECK(std::is_same<typename array_type::pointer, int*>::value);
  CHECK(std::is_same<typename array_type::const_pointer, const int*>::value);
  CHECK(std::is_same<typename array_type::iterator, int*>::value);
  CHECK(std::is_same<typename array_type::const_iterator, const int*>::value);
  CHECK(std::is_same<typename array_type::reverse_iterator, std::reverse_iterator<int*>>::value);
  CHECK(std::is_same<typename array_type::const_reverse_iterator, std::reverse_iterator<const int*>>::value);
}

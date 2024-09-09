/**
 * Copyright (C) 2021 - Marcel Breyer - All Rights Reserved
 * Licensed under the MIT License. See LICENSE.md file in the project root for full license information.
 *
 * Implements a runtime fixed-size array.
 */

#ifndef CPP_UTIL_DYNARRAY_HPP
#define CPP_UTIL_DYNARRAY_HPP

#include <algorithm>         // std::fill, std::copy, std::swap, std::generate, std::equal, std::lexicographical_compare_three_way, std::lexicographical_compare
#include <cassert>           // assert
#include <cstddef>           // std::size_t, std::ptrdiff_t
#include <initializer_list>  // std::initializer_list
#include <iterator>          // std::reverse_iterator, std::distance, std::make_reverse_iterator, std::iterator_traits, std::forward_iterator_tag
#include <limits>            // std::numeric_limits
#include <memory>            // std::addressof
#include <numeric>           // std::iota
#include <stdexcept>         // std::out_of_range
#include <type_traits>       // std::remove_cv, std::enable_if, std::is_convertible
#include <utility>           // std::exchange

#if __has_include(<compare>)
#include <compare>  // std::strong_ordering
#endif

#if defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard)
#define DYNARRAY_NODISCARD [[nodiscard]]
#else
#define DYNARRAY_NODISCARD
#endif

#if defined(__cpp_constexpr_dynamic_alloc) && defined(__cpp_lib_constexpr_algorithms) && defined(__cpp_lib_constexpr_numeric) && \
    defined(__cpp_constexpr) && defined(__cpp_lib_array_constexpr)
#define DYNARRAY_CONSTEXPR constexpr
#else
#define DYNARRAY_CONSTEXPR
#endif

namespace cpp_util {

namespace detail {

#if defined(__cpp_lib_make_reverse_iterator)
using std::make_reverse_iterator;
#else
// see: https://en.cppreference.com/w/cpp/iterator/make_reverse_iterator
template <class Iter>
constexpr std::reverse_iterator<Iter> make_reverse_iterator(Iter i) {
  return std::reverse_iterator<Iter>(i);
}
#endif

#if defined(__cpp_lib_exchange_function)
using std::exchange;
#else
// see: https://en.cppreference.com/w/cpp/utility/exchange
template <class T, class U = T>
T exchange(T& obj, U&& new_value) {
  T old_value = std::move(obj);
  obj = std::forward<U>(new_value);
  return old_value;
}
#endif

}  // namespace detail

template <typename T>
class dynarray {
 public:
  /**************************************************************************************************************************************/
  /**                                                              types                                                               **/
  /**************************************************************************************************************************************/
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using iterator = pointer;
  using const_iterator = const_pointer;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  static_assert(std::is_same<typename std::remove_cv<value_type>::type, value_type>::value,
                "cpp_util::dynarray must have a non-const, non-volatile value_type");

  /**************************************************************************************************************************************/
  /**                                                           construction                                                           **/
  /**************************************************************************************************************************************/
  DYNARRAY_CONSTEXPR dynarray() noexcept = default;
  DYNARRAY_CONSTEXPR explicit dynarray(const size_type size) : size_{ size }, data_{ new value_type[size] } {}
  DYNARRAY_CONSTEXPR dynarray(const size_type size, const value_type& init) : dynarray(size) {
    // initialize with same value
    this->fill(init);
  }
  template <typename ForwardIt, typename std::enable_if<std::is_convertible<typename std::iterator_traits<ForwardIt>::iterator_category,
                                                                            std::forward_iterator_tag>::value,
                                                        bool>::type = true>
  DYNARRAY_CONSTEXPR dynarray(ForwardIt first, ForwardIt last) : dynarray(static_cast<size_type>(std::distance(first, last))) {
    // copy values from iterator range
    std::copy(first, last, this->begin());
  }
  DYNARRAY_CONSTEXPR dynarray(std::initializer_list<value_type> ilist) : dynarray{ ilist.begin(), ilist.end() } {}
  DYNARRAY_CONSTEXPR dynarray(const dynarray& other) : dynarray(other.size_) {
    // copy values from other
    std::copy(other.cbegin(), other.cend(), this->begin());
  }
  DYNARRAY_CONSTEXPR dynarray(dynarray&& other) noexcept
      : size_{ detail::exchange(other.size_, size_type{ 0 }) }, data_{ detail::exchange(other.data_, nullptr) } {}

  /**************************************************************************************************************************************/
  /**                                                           destruction                                                            **/
  /**************************************************************************************************************************************/
  DYNARRAY_CONSTEXPR ~dynarray() {
    delete[] data_;
    size_ = 0;
  }

  /**************************************************************************************************************************************/
  /**                                                            assignment                                                            **/
  /**************************************************************************************************************************************/
  DYNARRAY_CONSTEXPR dynarray& operator=(const dynarray& other) {
    // guard against self assignment
    if (this != std::addressof(other)) {
      // if sizes mismatch use copy-and-swap idiom,
      // otherwise just directly assign new values
      if (other.size_ != size_) {
        dynarray tmp{ other };
        this->swap(tmp);
      } else {
        // perform copy
        std::copy(other.cbegin(), other.cend(), this->begin());
      }
    }
    return *this;
  }
  DYNARRAY_CONSTEXPR dynarray& operator=(dynarray&& other) noexcept {
    dynarray tmp{ std::move(other) };
    this->swap(tmp);
    return *this;
  }
  DYNARRAY_CONSTEXPR dynarray& operator=(std::initializer_list<value_type> ilist) {
    // if sizes mismatch use copy-and-swap idiom,
    // otherwise just directly assign new values
    if (ilist.size() != size_) {
      dynarray tmp{ ilist };
      this->swap(tmp);
    } else {
      // perform assignment
      std::copy(ilist.begin(), ilist.end(), this->begin());
    }
    return *this;
  }

  DYNARRAY_CONSTEXPR void assign(const size_type count, const value_type& value) {
    // if sizes mismatch use copy-and-swap idiom,
    // otherwise just directly assign new values
    if (count != size_) {
      dynarray tmp(count, value);
      this->swap(tmp);
    } else {
      // perform assignment
      this->fill(value);
    }
  }
  template <typename ForwardIt, typename std::enable_if<std::is_convertible<typename std::iterator_traits<ForwardIt>::iterator_category,
                                                                            std::forward_iterator_tag>::value,
                                                        bool>::type = true>
  DYNARRAY_CONSTEXPR void assign(ForwardIt first, ForwardIt last) {
    // if sizes mismatch use copy-and-swap idiom,
    // otherwise just directly assign new values
    if (static_cast<size_type>(std::distance(first, last)) != size_) {
      dynarray tmp(first, last);
      this->swap(tmp);
    } else {
      // perform assignment
      std::copy(first, last, this->begin());
    }
  }
  DYNARRAY_CONSTEXPR void assign(std::initializer_list<value_type> ilist) { this->assign(ilist.begin(), ilist.end()); }

  /**************************************************************************************************************************************/
  /**                                                          element access                                                          **/
  /**************************************************************************************************************************************/
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR reference at(const size_type pos) {
    if (pos >= size_) throw std::out_of_range{"Index out-of-range: pos >= this->size()"};
    return data_[pos];
  }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR const_reference at(const size_type pos) const {
    if (pos >= size_) throw std::out_of_range{"Index out-of-range: pos >= this->size()"};
    return data_[pos];
  }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR reference operator[](const size_type pos) {
    assert((pos < this->size()) && "Undefined behavior if pos >= this->size()!");
    return data_[pos];
  }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR const_reference operator[](const size_type pos) const {
    assert((pos < this->size()) && "Undefined behavior if pos >= this->size()!");
    return data_[pos];
  }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR reference front() {
    assert((!this->empty()) && "Calling front() is undefined for empty dynarrays!");
    return data_[0];
  }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR const_reference front() const {
    assert((!this->empty()) && "Calling front() is undefined for empty dynarrays!");
    return data_[0];
  }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR reference back() {
    assert((!this->empty()) && "Calling back() is undefined for empty dynarrays!");
    return data_[size_ - 1];
  }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR const_reference back() const {
    assert((!this->empty()) && "Calling back() is undefined for empty dynarrays!");
    return data_[size_ - 1];
  }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR pointer data() { return data_; }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR const_pointer data() const { return data_; }

  /**************************************************************************************************************************************/
  /**                                                         iterator support                                                         **/
  /**************************************************************************************************************************************/
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR iterator begin() noexcept { return data_; }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR iterator end() noexcept { return data_ + size_; }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR const_iterator begin() const noexcept { return data_; }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR const_iterator end() const noexcept { return data_ + size_; }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR const_iterator cbegin() const noexcept { return data_; }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR const_iterator cend() const noexcept { return data_ + size_; }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR reverse_iterator rbegin() noexcept { return detail::make_reverse_iterator(this->end()); }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR reverse_iterator rend() noexcept { return detail::make_reverse_iterator(this->begin()); }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR const_reverse_iterator rbegin() const noexcept {
    return detail::make_reverse_iterator(this->end());
  }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR const_reverse_iterator rend() const noexcept {
    return detail::make_reverse_iterator(this->begin());
  }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR const_reverse_iterator crbegin() const noexcept {
    return detail::make_reverse_iterator(this->end());
  }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR const_reverse_iterator crend() const noexcept {
    return detail::make_reverse_iterator(this->begin());
  }

  /**************************************************************************************************************************************/
  /**                                                             capacity                                                             **/
  /**************************************************************************************************************************************/
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR bool empty() const noexcept { return size_ == 0; }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR size_type size() const noexcept { return size_; }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR static size_type max_size() noexcept {
    return std::numeric_limits<difference_type>::max() / sizeof(value_type);
  }

  /**************************************************************************************************************************************/
  /**                                                            operations                                                            **/
  /**************************************************************************************************************************************/
  DYNARRAY_CONSTEXPR void swap(dynarray& other) noexcept {
    std::swap(size_, other.size_);
    std::swap(data_, other.data_);
  }
  DYNARRAY_CONSTEXPR void fill(const value_type& value = value_type{}) {
    assert((data_ != nullptr) && "Calling fill() is undefined for nullptr data!");
    std::fill(this->begin(), this->end(), value);
  }
  DYNARRAY_CONSTEXPR void iota(const value_type& value = value_type{}) {
    assert((data_ != nullptr) && "Calling iota() is undefined for nullptr data!");
    std::iota(this->begin(), this->end(), value);
  }
  template <typename Generator>
  DYNARRAY_CONSTEXPR void generate(Generator gen) {
    assert((data_ != nullptr) && "Calling generate() is undefined for nullptr data!");
    std::generate(this->begin(), this->end(), gen);
  }

  /**************************************************************************************************************************************/
  /**                                                       non-member functions                                                       **/
  /**************************************************************************************************************************************/
#if defined(__cpp_impl_three_way_comparison) && defined(__cpp_lib_three_way_comparison)
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR friend bool operator==(const dynarray& lhs, const dynarray& rhs) noexcept {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
  }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR friend std::strong_ordering operator<=>(const dynarray& lhs, const dynarray& rhs) noexcept {
    return lhs.size() != rhs.size() ? lhs.size() <=> rhs.size()
                                    : std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
  }
#else
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR friend bool operator==(const dynarray& lhs, const dynarray& rhs) noexcept {
#if defined(__cpp_lib_robust_nonmodifying_seq_ops)
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
#else
    if (std::distance(lhs.cbegin(), lhs.cend()) != std::distance(rhs.cbegin(), rhs.cend())) {
      return false;
    } else {
      return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
    }
#endif
  }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR friend bool operator!=(const dynarray& lhs, const dynarray& rhs) noexcept { return !(lhs == rhs); }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR friend bool operator<(const dynarray& lhs, const dynarray& rhs) noexcept {
    return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
  }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR friend bool operator>(const dynarray& lhs, const dynarray& rhs) noexcept { return rhs < lhs; }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR friend bool operator<=(const dynarray& lhs, const dynarray& rhs) noexcept { return !(rhs < lhs); }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR friend bool operator>=(const dynarray& lhs, const dynarray& rhs) noexcept { return !(lhs < rhs); }
#endif

 private:
  size_type size_{ 0 };
  pointer data_{ nullptr };
};

template <typename T>
DYNARRAY_CONSTEXPR void swap(dynarray<T>& lhs, dynarray<T>& rhs) noexcept { lhs.swap(rhs); }

/****************************************************************************************************************************************/
/**                                                          deduction guides                                                          **/
/****************************************************************************************************************************************/
#if defined(__cpp_deduction_guides)
template <typename ForwardIt>
dynarray(ForwardIt, ForwardIt) -> dynarray<typename std::iterator_traits<ForwardIt>::value_type>;
#endif

}  // namespace cpp_util

#undef DYNARRAY_NODISCARD
#undef DYNARRAY_CONSTEXPR

#endif  // CPP_UTIL_DYNARRAY_HPP

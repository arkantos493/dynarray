// (c) 2021 Marcel Breyer
// This code is licensed under MIT license (see LICENSE.md for details)

#ifndef DYNARRAY_HPP
#define DYNARRAY_HPP

#include <algorithm>         // std::fill, std::copy, std::swap, std::generate, std::equal, std::
#include <cassert>           // assert
#include <cstddef>           // std::size_t, std::ptrdiff_t
#include <initializer_list>  // std::initializer_list
#include <iterator>          // std::reverse_iterator, std::distance, std::make_reverse_iterator, std::iterator_traits
#include <limits>            // std::numeric_limits
#include <memory>            // std::addressof
#include <numeric>           // std::iota
#include <stdexcept>         // std::out_of_range
#include <type_traits>       // std::enable_if_t
#include <utility>           // std::exchange

#if __cplusplus >= 201703L
#define DYNARRAY_NODISCARD [[nodiscard]]
#else
#define DYNARRAY_NODISCARD
#endif

#if __cplusplus >= 202002L
#define DYNARRAY_CONSTEXPR constexpr
#else
#define DYNARRAY_CONSTEXPR
#endif

namespace util {

template <typename T>
class dynarray {
 public:
  /*******************************************************************************************************************/
  /**                                                     types                                                     **/
  /*******************************************************************************************************************/
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

  /*******************************************************************************************************************/
  /**                                                 construction                                                  **/
  /*******************************************************************************************************************/
  DYNARRAY_CONSTEXPR dynarray() = default;
  DYNARRAY_CONSTEXPR explicit dynarray(size_type size) : size_{size}, data_{new value_type[size]} {}
  DYNARRAY_CONSTEXPR dynarray(size_type size, const value_type& init) : size_{size}, data_{new value_type[size]} {
    // initialize with same value
    std::fill(this->begin(), this->end(), init);
  }
  template <typename InputIt,
            std::enable_if_t<std::is_convertible_v<typename std::iterator_traits<InputIt>::iterator_category, std::input_iterator_tag>,
                             bool> = true>
  DYNARRAY_CONSTEXPR dynarray(InputIt first, InputIt last)
      : size_{static_cast<size_type>(std::distance(first, last))}, data_{new value_type[size_]} {
    // copy values from iterator range
    std::copy(first, last, this->begin());
  }
  DYNARRAY_CONSTEXPR dynarray(std::initializer_list<value_type> ilist) : dynarray{ilist.begin(), ilist.end()} {}
  DYNARRAY_CONSTEXPR dynarray(const dynarray& other) : size_{other.size_}, data_{new value_type[size_]} {
    // copy values from other
    std::copy(other.cbegin(), other.cend(), this->begin());
  }
  DYNARRAY_CONSTEXPR dynarray(dynarray&& other) noexcept
      : size_{std::exchange(other.size_, 0)}, data_{std::exchange(other.data_, nullptr)} {}

  /*******************************************************************************************************************/
  /**                                                  destruction                                                  **/
  /*******************************************************************************************************************/
  DYNARRAY_CONSTEXPR ~dynarray() { delete[] data_; }

  /*******************************************************************************************************************/
  /**                                                  assignment                                                   **/
  /*******************************************************************************************************************/
  DYNARRAY_CONSTEXPR dynarray& operator=(const dynarray& other) {
    // guard against self assignment
    if (this != std::addressof(other)) {
      // if sizes mismatch use copy-and-swap idiom,
      // otherwise just directly assign new values
      if (other.size_ != size_) {
        dynarray tmp{other};
        this->swap(tmp);
      } else {
        // perform copy
        std::copy(other.cbegin(), other.cend(), this->begin());
      }
    }
    return *this;
  }
  DYNARRAY_CONSTEXPR dynarray& operator=(dynarray&& other) noexcept {
    // guard against self assignment
    if (this != std::addressof(other)) {
      // free resources
      delete[] data_;
      // perform move and set other to moved-from state
      size_ = std::exchange(other.size_, 0);
      data_ = std::exchange(other.data_, nullptr);
    }
    return *this;
  }
  DYNARRAY_CONSTEXPR dynarray& operator=(std::initializer_list<value_type> ilist) {
    // if sizes mismatch use copy-and-swap idiom,
    // otherwise just directly assign new values
    if (ilist.size() != size_) {
      dynarray tmp{ilist};
      this->swap(tmp);
    } else {
      // perform assignment
      std::copy(ilist.begin(), ilist.end(), this->begin());
    }
    return *this;
  }

  /*******************************************************************************************************************/
  /**                                                 element access                                                **/
  /*******************************************************************************************************************/
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR reference at(const size_type pos) {
    if (pos >= size_) throw std::out_of_range{"Index out-of-range: pos >= this->size()"};
    return data_[pos];
  }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR const_reference at(const size_type pos) const {
    if (pos >= size_) throw std::out_of_range{"Index out-of-range: pos >= this->size()"};
    return data_[pos];
  }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR reference operator[](const size_type pos) {
    assert((pos < this->size()) && "Undefined behavior if pos >= size()!");
    return data_[pos];
  }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR const_reference operator[](const size_type pos) const {
    assert((pos < this->size()) && "Undefined behavior if pos >= size()!");
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

  /*******************************************************************************************************************/
  /**                                                iterator support                                               **/
  /*******************************************************************************************************************/
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR iterator begin() noexcept { return data_; }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR iterator end() noexcept { return data_ + size_; }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR const_iterator begin() const noexcept { return data_; }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR const_iterator end() const noexcept { return data_ + size_; }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR const_iterator cbegin() const noexcept { return data_; }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR const_iterator cend() const noexcept { return data_ + size_; }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR reverse_iterator rbegin() noexcept { return std::make_reverse_iterator(this->end()); }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR reverse_iterator rend() noexcept { return std::make_reverse_iterator(this->begin()); }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR const_reverse_iterator rbegin() const noexcept { return std::make_reverse_iterator(this->end()); }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR const_reverse_iterator rend() const noexcept { return std::make_reverse_iterator(this->begin()); }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR const_reverse_iterator crbegin() const noexcept { return std::make_reverse_iterator(this->end()); }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR const_reverse_iterator crend() const noexcept { return std::make_reverse_iterator(this->begin()); }

  /*******************************************************************************************************************/
  /**                                                   capacity                                                    **/
  /*******************************************************************************************************************/
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR bool empty() const noexcept { return size_ == 0; }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR size_type size() const noexcept { return size_; }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR size_type max_size() const noexcept { return std::numeric_limits<difference_type>::max(); }

  /*******************************************************************************************************************/
  /**                                                   operations                                                  **/
  /*******************************************************************************************************************/
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

  /*******************************************************************************************************************/
  /**                                              non-member functions                                             **/
  /*******************************************************************************************************************/
  DYNARRAY_CONSTEXPR friend void swap(dynarray& lhs, dynarray& rhs) noexcept { lhs.swap(rhs); }
#if __cplusplus >= 202002L
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR friend bool operator==(const dynarray& lhs, const dynarray& rhs) noexcept {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
  }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR friend bool operator==(const dynarray& lhs, const dynarray& rhs) noexcept {
    return lhs.size() != rhs.size() ? lhs.size() <=> rhs.size()
                                    : std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
  }
#else
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR friend bool operator==(const dynarray& lhs, const dynarray& rhs) noexcept {
    return lhs.compare(rhs) == 0;
  }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR friend bool operator!=(const dynarray& lhs, const dynarray& rhs) noexcept {
    return lhs.compare(rhs) != 0;
  }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR friend bool operator<(const dynarray& lhs, const dynarray& rhs) noexcept {
    return lhs.compare(rhs) < 0;
  }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR friend bool operator>(const dynarray& lhs, const dynarray& rhs) noexcept {
    return lhs.compare(rhs) > 0;
  }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR friend bool operator<=(const dynarray& lhs, const dynarray& rhs) noexcept {
    return lhs.compare(rhs) <= 0;
  }
  DYNARRAY_NODISCARD DYNARRAY_CONSTEXPR friend bool operator>=(const dynarray& lhs, const dynarray& rhs) noexcept {
    return lhs.compare(rhs) >= 0;
  }
#endif

 private:
#if __cplusplus < 202002L
  int compare(const dynarray& other) const noexcept {
    if (size_ != other.size_) {
      return size_ > other.size_ ? 1 : -1;
    }
    for (size_type i = 0; i < size_; ++i) {
      if (data_[i] > other.data_[i]) {
        return 1;
      } else if (data_[i] < other.data_[i]) {
        return -1;
      }
    }
    return 0;
  }
#endif

  size_type size_{0};
  pointer data_{nullptr};
};

/*******************************************************************************************************************/
/**                                               deduction guides                                                **/
/*******************************************************************************************************************/
#if __cplusplus >= 201703L
template <typename InputIt>
dynarray(InputIt, InputIt) -> dynarray<typename std::iterator_traits<InputIt>::value_type>;
#endif

}  // namespace util

#undef DYNARRAY_NODISCARD
#undef DYNARRAY_CONSTEXPR

#endif  // DYNARRAY_HPP
// (c) 2021 Marcel Breyer
// This code is licensed under MIT license (see LICENSE.md for details)

#ifndef DYNARRAY_HPP
#define DYNARRAY_HPP

#include <algorithm>         // std::fill, std::copy, std::swap, std::generate, std::equal
#include <cstddef>           // std::size_t, std::ptrdiff_t
#include <initializer_list>  // std::initializer_list
#include <iterator>          // std::reverse_iterator, std::input_iterator, std::distance, std::make_reverse_iterator, std::iterator_traits
#include <memory>            // std::addressof
#include <numeric>           // std::iota
#include <stdexcept>         // std::out_of_range
#include <type_traits>       // std::is_invocable_r_v
#include <utility>           // std::exchange

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
  constexpr dynarray() : size_{0}, data_{nullptr} {}
  constexpr explicit dynarray(const size_type size) : size_{size}, data_{new value_type[size]} {}
  constexpr dynarray(const size_type size, const value_type& init) : size_{size}, data_{new value_type[size]} {
    // initialize with same value
    std::fill(this->begin(), this->end(), init);
  }
  template <std::input_iterator InputIt>
  constexpr dynarray(InputIt first, InputIt last)
      : size_{static_cast<size_type>(std::distance(first, last))}, data_{new value_type[size_]} {
    // copy values from iterator range
    std::copy(first, last, this->begin());
  }
  constexpr dynarray(std::initializer_list<value_type> ilist) : dynarray{ilist.begin(), ilist.end()} {}
  constexpr dynarray(const dynarray& other) : size_{other.size_}, data_{new value_type[size_]} {
    // copy values from other
    std::copy(other.cbegin(), other.cend(), this->begin());
  }
  constexpr dynarray(dynarray&& other) noexcept : size_{std::exchange(other.size_, 0)}, data_{std::exchange(other.data_, nullptr)} {}

  /*******************************************************************************************************************/
  /**                                                  destruction                                                  **/
  /*******************************************************************************************************************/
  constexpr ~dynarray() { delete[] data_; }

  /*******************************************************************************************************************/
  /**                                                  assignment                                                   **/
  /*******************************************************************************************************************/
  constexpr dynarray& operator=(const dynarray& other) {
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
  constexpr dynarray& operator=(dynarray&& other) noexcept {
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
  constexpr dynarray& operator=(std::initializer_list<value_type> ilist) {
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
  [[nodiscard]] constexpr reference at(const size_type pos) {
    if (pos >= size_) throw std::out_of_range{"Index out-of-range: pos >= this->size()"};
    return data_[pos];
  }
  [[nodiscard]] constexpr const_reference at(const size_type pos) const {
    if (pos >= size_) throw std::out_of_range{"Index out-of-range: pos >= this->size()"};
    return data_[pos];
  }
  [[nodiscard]] constexpr reference operator[](const size_type pos) { return data_[pos]; }
  [[nodiscard]] constexpr const_reference operator[](const size_type pos) const { return data_[pos]; }
  [[nodiscard]] constexpr reference front() { return data_[0]; }
  [[nodiscard]] constexpr const_reference front() const { return data_[0]; }
  [[nodiscard]] constexpr reference back() { return data_[size_ - 1]; }
  [[nodiscard]] constexpr const_reference back() const { return data_[size_ - 1]; }
  [[nodiscard]] constexpr pointer data() { return data_; }
  [[nodiscard]] constexpr const_pointer data() const { return data_; }

  /*******************************************************************************************************************/
  /**                                                iterator support                                               **/
  /*******************************************************************************************************************/
  [[nodiscard]] constexpr iterator begin() noexcept { return data_; }
  [[nodiscard]] constexpr iterator end() noexcept { return data_ + size_; }
  [[nodiscard]] constexpr const_iterator begin() const noexcept { return data_; }
  [[nodiscard]] constexpr const_iterator end() const noexcept { return data_ + size_; }
  [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return data_; }
  [[nodiscard]] constexpr const_iterator cend() const noexcept { return data_ + size_; }
  [[nodiscard]] constexpr reverse_iterator rbegin() noexcept { return std::make_reverse_iterator(this->end()); }
  [[nodiscard]] constexpr reverse_iterator rend() noexcept { return std::make_reverse_iterator(this->begin()); }
  [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept { return std::make_reverse_iterator(this->end()); }
  [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept { return std::make_reverse_iterator(this->begin()); }
  [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept { return std::make_reverse_iterator(this->end()); }
  [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept { return std::make_reverse_iterator(this->begin()); }

  /*******************************************************************************************************************/
  /**                                                   capacity                                                    **/
  /*******************************************************************************************************************/
  [[nodiscard]] constexpr bool empty() const noexcept { return size_ == 0; }
  [[nodiscard]] constexpr size_type size() const noexcept { return size_; }
  [[nodiscard]] constexpr size_type max_size() const noexcept { return std::numeric_limits<difference_type>::max(); }

  /*******************************************************************************************************************/
  /**                                                   operations                                                  **/
  /*******************************************************************************************************************/
  constexpr void swap(dynarray& other) noexcept {
    std::swap(size_, other.size_);
    std::swap(data_, other.data_);
  }
  constexpr void fill(const value_type& value = value_type{}) { std::fill(this->begin(), this->end(), value); }
  constexpr void iota(const value_type& value = value_type{}) { std::iota(this->begin(), this->end(), value); }
  template <typename Generator>
  requires std::is_invocable_r_v<value_type, Generator>
  constexpr void generate(Generator gen) { std::generate(this->begin(), this->end(), gen); }

  /*******************************************************************************************************************/
  /**                                              non-member functions                                             **/
  /*******************************************************************************************************************/
  constexpr friend void swap(dynarray& lhs, dynarray& rhs) noexcept { lhs.swap(rhs); }
  [[nodiscard]] constexpr friend bool operator==(const dynarray& lhs, const dynarray& rhs) noexcept {
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
  }
  [[nodiscard]] constexpr friend auto operator<=>(const dynarray& lhs, const dynarray& rhs) noexcept {
    // dynarrays are only equal if their sizes are equal
    const auto size_cmp = lhs.size_ <=> rhs.size_;
    if (size_cmp != 0) return size_cmp;
    for (size_type i = 0; i < lhs.size_; ++i) {
      // check for elements-wise equality
      if (auto cmp = lhs.data_[i] <=> rhs.data_[i]; cmp != 0) return cmp;
    }
    // dynarrays are equal (as was size_cmp previously)
    return size_cmp;
  }

 private:
  size_type size_;
  pointer data_;
};

/*******************************************************************************************************************/
/**                                               deduction guides                                                **/
/*******************************************************************************************************************/
template <std::input_iterator InputIt>
dynarray(InputIt, InputIt) -> dynarray<typename std::iterator_traits<InputIt>::value_type>;

}  // namespace util

#endif  // DYNARRAY_HPP
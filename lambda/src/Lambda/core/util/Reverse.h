/// @file Reverse.h
/// @brief A lightweight utility for Reversing STL based containers.
#ifndef LAMBDA_SRC_LAMBDA_CORE_UTIL_REVERSE_H_
#define LAMBDA_SRC_LAMBDA_CORE_UTIL_REVERSE_H_

namespace lambda::core::util {

/// @brief A clean container for iterating through any container that implements
/// rbegin and rend.
template<class Container>
class Reverse {
 public:
  explicit Reverse(Container& container) : container_(container) {}
  auto begin() { return container_.rbegin(); }
  auto end() { return container_.rend(); }

 private:
  Container& container_;
};

}  // namespace lambda::core::util

#endif  // LAMBDA_SRC_LAMBDA_CORE_UTIL_REVERSE_H_

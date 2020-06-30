/**
 * @file engine/src/core/util/Reverse.h
 * @brief A lightweight utility for Reversing STL based containers.
 */
#ifndef ENGINE_SRC_CORE_UTIL_REVERSE_H_
#define ENGINE_SRC_CORE_UTIL_REVERSE_H_

#include <ranges>

namespace engine {
namespace util {

namespace internal {

/**
 * C++20 constraint to require template types to be iterable.
 */
template<class Container>
concept Iterable = std::ranges::bidirectional_range<Container>;

}  // namespace internal

/**
 * @class Reverse
 * @brief Provides a clean interface for iterating through any container that
 * implements rbegin and rend.
 */
template<internal::Iterable Container>
class Reverse {
 public:
  explicit Reverse(Container& container) : container_(container) {}
  auto begin() { return container_.rbegin(); }
  auto end() { return container_.rend(); }
 private:
  Container& container_;
};

}  // namespace util
}  // namespace engine

#endif  // ENGINE_SRC_CORE_UTIL_REVERSE_H_
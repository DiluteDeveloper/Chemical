#pragma once

namespace Chemical {

  template <typename T> using SafePtr = T *;

  // // Intended to mitigate include loops;
  // // Means that the pointer must be valid,
  // // but is still a pointer
  // template <typename T> class SafePtr {
  // public:
  //   SafePtr(T &value) : value(&value) {}
  //
  //   T &operator->() {
  //     return value;
  //   }
  //
  // private:
  //   T *value = nullptr;
  // };
} // namespace Chemical

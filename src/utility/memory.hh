#pragma once

#include <concepts>

namespace corona::utility::memory
{
  /// \brief Copies bytes represented by source pointer into destination pointer of type <code>D</code>.
  /// \tparam D Destination type. Must satisfy <code>std::semiregular</code> constraint, i.e. have a
  /// default constructor, and be trivially copyable and movable.
  /// \tparam S Source type.
  /// \param source Source pointer.
  /// \returns Destination type object.
  template <std::semiregular D, typename S>
  [[nodiscard]] auto byte_cast(S const* source) -> D {
    if(source == nullptr)
      throw std::invalid_argument("byte_cast: source cannot be null");
    auto dest = D();
    std::memcpy(&dest, source, sizeof dest);
    return dest;
  }
} // namespace corona::utility::memory
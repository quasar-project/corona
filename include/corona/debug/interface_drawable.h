#pragma once

#include <corona/detail/export.h>

namespace corona
{
  class IDrawableOnImmediateGUI
  {
   public:
    virtual ~IDrawableOnImmediateGUI() = default;
    virtual auto frame() -> void = 0;
  };

  template <typename T>
  concept IDrawableOnImmediateGUIConcept = std::is_base_of_v<IDrawableOnImmediateGUI, T>;
} // namespace corona
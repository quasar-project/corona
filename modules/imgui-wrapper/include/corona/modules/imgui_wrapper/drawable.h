#pragma once

#include "detail/export.h"

namespace corona::modules::imgui
{
  class IDrawable
  {
   public:
    virtual ~IDrawable() = default;
    virtual auto frame() -> void = 0;
  };

  template <typename T>
  concept IDrawableConcept = std::is_base_of_v<IDrawable, T>;
} // namespace corona::modules::imgui
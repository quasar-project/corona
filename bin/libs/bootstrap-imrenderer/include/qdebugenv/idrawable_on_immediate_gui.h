#pragma once

#include <floppy/floppy.h>

namespace qdebugenv
{
  class IDrawableOnImmediateGui
  {
   public:
    virtual ~IDrawableOnImmediateGui() = default;
    virtual auto frame() -> void = 0;
  };

  template <typename T>
  concept IDrawableOnImmediateGuiConcept = std::is_base_of_v<IDrawableOnImmediateGui, T>;
} // qdebugenv
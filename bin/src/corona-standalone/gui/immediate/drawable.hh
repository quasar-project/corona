#pragma once

#include <floppy/floppy.h>

namespace corona::standalone::gui::immediate
{
  class Drawable
  {
   public:
    virtual ~Drawable() = default;
    virtual auto draw_debug_frame() -> void = 0;
  };

  template<typename T>
  concept DrawableConcept = std::is_base_of_v<Drawable, T>;
} // namespace corona::standalone::gui::immediate
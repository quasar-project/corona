#pragma once

#include "../detail/export.h"

namespace corona::bootstrap::imrenderer
{
  class CRhiRenderer;
  class CRhiImmediateGuiCustomRenderer
  {
   public:
    virtual ~CRhiImmediateGuiCustomRenderer();
    virtual auto sync(CRhiRenderer* renderer) -> void;
    virtual auto render() -> void;
  };
} // namespace corona::bootstrap::imrenderer
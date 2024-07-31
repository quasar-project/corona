#pragma once

#include <qdebugenv/detail/export.h>

namespace qdebugenv
{
  class CRhiRenderer;
  class CRhiImmediateGuiCustomRenderer
  {
   public:
    virtual ~CRhiImmediateGuiCustomRenderer();
    virtual auto sync(CRhiRenderer* renderer) -> void;
    virtual auto render() -> void;
  };
} // namespace qdebugenv
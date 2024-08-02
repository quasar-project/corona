#pragma once

#include <vector>
#include <functional>
#include "detail/export.h"
#include "class_generic_renderer.h"
#include <corona/modules/imgui_wrapper/drawable.h>

namespace corona::bootstrap::imrenderer
{
  // todo: rm
  using namespace modules;
  class CORONA_BOOTSTRAP_IMRENDERER_API CExtendableRenderer : public CGenericRenderer // NOLINT(*-special-member-functions)
  {
    Q_OBJECT

   public:
    explicit CExtendableRenderer(CGenericRenderer* parent = nullptr);
    ~CExtendableRenderer() override;

    [[nodiscard]] auto callbacks() const -> std::vector<std::function<void()>> const& { return this->callbacks_; }
    [[nodiscard]] auto callbacks_mut() -> std::vector<std::function<void()>>& { return this->callbacks_; }
    [[nodiscard]] auto drawables() const -> std::vector<imgui::IDrawable*> const& { return this->drawables_; }
    [[nodiscard]] auto drawables_mut() -> std::vector<imgui::IDrawable*>& { return this->drawables_; }

    auto frame() -> void override;
    auto add_callback(std::function<void()> const& callback) -> void { this->callbacks_.push_back(callback); }
    auto add_drawable(imgui::IDrawable* drawable) -> void { this->drawables_.push_back(drawable); }

    auto operator+=(std::function<void()> const& callback) -> void { this->add_callback(callback); }
    auto operator+=(imgui::IDrawable* drawable) -> void { this->add_drawable(drawable); }

   private:
    std::vector<std::function<void()>> callbacks_;
    std::vector<imgui::IDrawable*> drawables_;
  };
} // namespace corona::bootstrap::imrenderer
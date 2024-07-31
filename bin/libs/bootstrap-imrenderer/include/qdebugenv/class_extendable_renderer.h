#pragma once

#include <vector>
#include <functional>
#include <qdebugenv/detail/export.h>
#include <qdebugenv/idrawable_on_immediate_gui.h>
#include <qdebugenv/class_generic_renderer.h>

namespace qdebugenv
{
  class QDE_API CExtendableRenderer : public CGenericRenderer // NOLINT(*-special-member-functions)
  {
    Q_OBJECT

   public:
    explicit CExtendableRenderer(CGenericRenderer* parent = nullptr);
    ~CExtendableRenderer() override;

    [[nodiscard]] auto callbacks() const -> std::vector<std::function<void()>> const& { return this->callbacks_; }
    [[nodiscard]] auto callbacks_mut() -> std::vector<std::function<void()>>& { return this->callbacks_; }
    [[nodiscard]] auto drawables() const -> std::vector<IDrawableOnImmediateGui*> const& { return this->drawables_; }
    [[nodiscard]] auto drawables_mut() -> std::vector<IDrawableOnImmediateGui*>& { return this->drawables_; }

    auto frame() -> void override;
    auto add_callback(std::function<void()> const& callback) -> void { this->callbacks_.push_back(callback); }
    auto add_drawable(IDrawableOnImmediateGui* drawable) -> void { this->drawables_.push_back(drawable); }

    auto operator+=(std::function<void()> const& callback) -> void { this->add_callback(callback); }
    auto operator+=(IDrawableOnImmediateGui* drawable) -> void { this->add_drawable(drawable); }

   private:
    std::vector<std::function<void()>> callbacks_;
    std::vector<IDrawableOnImmediateGui*> drawables_;
  };
} // namespace qdebugenv
#pragma once

#include <vector>
#include <functional>
#include <qrhiimgui/qrhiimguiitem.h>
#include <corona-standalone/gui/immediate/drawable.hh>

namespace corona::standalone::gui::immediate
{
  class GenericItem : public QRhiImguiItem // NOLINT(*-special-member-functions)
  {
    Q_OBJECT

   public:
    explicit GenericItem(QRhiImguiItem* parent = nullptr);
    ~GenericItem() override;

    [[nodiscard]] auto callbacks() const -> std::vector<std::function<void()>> const& { return this->callbacks_; }
    [[nodiscard]] auto callbacks_mut() -> std::vector<std::function<void()>>& { return this->callbacks_; }
    [[nodiscard]] auto drawables() const -> std::vector<Drawable*> const& { return this->drawables_; }
    [[nodiscard]] auto drawables_mut() -> std::vector<Drawable*>& { return this->drawables_; }

    auto frame() -> void override;
    auto add_callback(std::function<void()> const& callback) -> void { this->callbacks_.push_back(callback); }
    auto add_drawable(Drawable* drawable) -> void { this->drawables_.push_back(drawable); }

    auto operator+=(std::function<void()> const& callback) -> void { this->add_callback(callback); }
    auto operator+=(Drawable* drawable) -> void { this->add_drawable(drawable); }

   private:
    std::vector<std::function<void()>> callbacks_;
    std::vector<Drawable*> drawables_;
  };
} // namespace corona::standalone::gui::immediate
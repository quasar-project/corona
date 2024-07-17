#pragma once

#include <vector>
#include <functional>
#include <qrhiimgui/qrhiimguiitem.h>

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

    auto frame() -> void override;

    auto operator+=(std::function<void()> const& callback) -> void { this->callbacks_.push_back(callback); }

   private:
    std::vector<std::function<void()>> callbacks_;
  };
} // namespace corona::standalone::gui::immediate
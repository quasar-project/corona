#pragma once

#include <qguiapplication.h>
#include <corona-standalone/definitions.hh>
#include <corona-standalone/app/iapplication.hh>
#include <corona-standalone/app/meta.hh>

namespace corona::standalone::app
{
  class Corona final : public IApplication<QGuiApplication> // NOLINT(*-special-member-functions) (reason: pinned)
  {
   public:
    explicit Corona(int& args, char** argv);
    virtual ~Corona() override;

    auto with_icon(string_view path) -> Corona&;
    auto with_quick_style(QuickStyle style) -> Corona&;

    [[nodiscard]] auto run_scene(string_view path) -> int;

   private:
    struct impl;
    fl::traits::pimpl<struct impl> impl_;
  };
} // namespace corona::standalone::app
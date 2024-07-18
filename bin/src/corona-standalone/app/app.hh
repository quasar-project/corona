#pragma once

#include <qguiapplication.h>
#include <corona-standalone/definitions.hh>
#include <corona-standalone/app/iapplication.hh>
#include <corona-standalone/app/meta.hh>

/* fwd */
class QQuickWindow;
namespace floppy::filesystem { class application_dirs; } // namespace floppy::filesystem
namespace corona::standalone
{
  namespace gui {
    namespace theme::qml { class ThemeWrapper; } // namespace theme::qml
    namespace immediate { class GenericItem; } // namespace immediate
  } // namespace gui

  namespace app
  {
    class Logger;
    class Corona final : public IApplication<QGuiApplication> // NOLINT(*-special-member-functions) (reason: pinned)
    {
     public:
      explicit Corona(int& args, char** argv, Logger& logger);
      virtual ~Corona() override; // NOLINT(*-use-override)

      auto with_icon(string_view path) -> Corona&;
      auto with_quick_style(QuickStyle style) -> Corona&;

      [[nodiscard]] auto run_scene(string_view path) -> int;

      [[nodiscard]] auto logger() const -> Logger const&;
      [[nodiscard]] auto logger_mut() -> Logger&;

      [[nodiscard]] auto dirs() const -> fl::filesystem::application_dirs const&;
      [[nodiscard]] auto dirs_mut() -> fl::filesystem::application_dirs&;

      [[nodiscard]] auto theme() const -> gui::theme::qml::ThemeWrapper const&;
      [[nodiscard]] auto theme_mut() -> gui::theme::qml::ThemeWrapper&;

      [[nodiscard]] auto imgui() const -> gui::immediate::GenericItem const&;
      [[nodiscard]] auto imgui_mut() -> gui::immediate::GenericItem&;

      [[nodiscard]] auto quick_window() const -> QQuickWindow*;

     private:
      struct impl;
      fl::traits::pimpl<struct impl> impl_;
    };
  } // namespace app
} // namespace corona::standalone
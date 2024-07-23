#pragma once

#include <qguiapplication.h>
#include <corona-standalone/definitions.hh>
#include <corona-standalone/app/interface_application.hh>
#include <corona-standalone/app/meta.hh>

/* fwd */
class QQuickWindow;
namespace floppy::filesystem { class application_dirs; } // namespace floppy::filesystem
namespace qdebugenv { class CExtendableRenderer; } // namespace qdebugenv
namespace corona::standalone
{
  namespace gui::theme::qml { class CThemeWrapper; } // namespace gui::theme::qml
  namespace app
  {
    class CLogger;
    class Corona final : public IApplication<QGuiApplication> // NOLINT(*-special-member-functions) (reason: pinned)
    {
     public:
      explicit Corona(int& args, char** argv, CLogger& logger);
      virtual ~Corona() override; // NOLINT(*-use-override)

      auto with_icon(string_view path) -> Corona&;
      auto with_quick_style(QuickStyle style) -> Corona&;

      [[nodiscard]] auto run_scene(string_view path) -> int;

      [[nodiscard]] auto logger() const -> CLogger const&;
      [[nodiscard]] auto logger_mut() -> CLogger&;

      [[nodiscard]] auto dirs() const -> fl::filesystem::application_dirs const&;
      [[nodiscard]] auto dirs_mut() -> fl::filesystem::application_dirs&;

      [[nodiscard]] auto theme() const -> gui::theme::qml::CThemeWrapper const&;
      [[nodiscard]] auto theme_mut() -> gui::theme::qml::CThemeWrapper&;

      [[nodiscard]] auto imgui() const -> qdebugenv::CExtendableRenderer const&;
      [[nodiscard]] auto imgui_mut() -> qdebugenv::CExtendableRenderer&;

     private:
      struct impl;
      fl::traits::pimpl<struct impl> impl_;
    };
  } // namespace app
} // namespace corona::standalone
#pragma once

#include <qguiapplication.h>
#include <corona-standalone/definitions.hh>
#include <corona-standalone/app/interface_application.hh>
#include <corona-standalone/app/meta.hh>

/* fwd */
class QQuickWindow;
namespace floppy { class application_dirs; } // namespace floppy
namespace corona::modules::imgui_renderer { class CExtendableRenderer; } // namespace corona::modules::imgui_renderer
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

      [[nodiscard]] auto dirs() const -> fl::application_dirs const&;
      [[nodiscard]] auto dirs_mut() -> fl::application_dirs&;

      [[nodiscard]] auto theme() const -> gui::theme::qml::CThemeWrapper const&;
      [[nodiscard]] auto theme_mut() -> gui::theme::qml::CThemeWrapper&;

      [[nodiscard]] auto imgui() const -> imgui_renderer::CExtendableRenderer const&;
      [[nodiscard]] auto imgui_mut() -> imgui_renderer::CExtendableRenderer&;

     private:
      auto register_qml_types() -> void;
      static auto load_plugins() -> void;

      struct impl;
      fl::pimpl<struct impl> impl_;
    };
  } // namespace app
} // namespace corona::standalone
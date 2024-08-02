#pragma once

#include <corona-standalone/app/app.hh>
#include <corona-standalone/utility/formatters.hh>

#include <corona/modules/extern/imterm/imterm.h>
#include <corona/modules/imgui_renderer/class_extendable_renderer.h>
#include <corona-standalone/app/class_dirs_wrapper.hh>
#include <corona-standalone/app/class_user_interface_logger.hh>
#include <corona-standalone/map/class_map_view_manager.hh>

class QQmlApplicationEngine;
namespace corona::standalone
{
  namespace gui
  {
    namespace immediate {
      class terminal_commands;
      struct custom_command_struct;
    } // namespace immediate
    namespace theme::qml { class CThemeWrapper; } // namespace theme::qml
  } // namespace gui

  namespace app
  {
    namespace detail
    {
      [[nodiscard]] auto platform_dependent_icon(std::string_view stem) -> ::QIcon;
      [[nodiscard]] auto qml_url(std::string_view path) -> ::QUrl;
    } // namespace detail

    struct ImGUIData
    {
      explicit ImGUIData(CLogger& logger);

      modules::imgui_renderer::CExtendableRenderer* imgui{nullptr};
      std::unique_ptr<gui::immediate::custom_command_struct> terminal_cmd;
      std::unique_ptr<modules::extern_::imterm::terminal<gui::immediate::terminal_commands>> terminal;
    };

    struct Corona::impl
    {
      explicit impl(CLogger& logger);
      ~impl();

      auto emplace_themes() -> void;
      auto configure_imgui(::QQmlApplicationEngine* engine) -> void;

      CLogger& logger; // NOLINT(*-avoid-const-or-ref-data-members)
      ImGUIData imgui;
      fl::box<gui::theme::qml::CThemeWrapper> theme;
      fl::box<qml::CApplicationDirsWrapper> app_dirs;
      fl::box<map::CMapViewManager> map_view_manager;
    };
  } // namespace app
} // namespace corona::standalone
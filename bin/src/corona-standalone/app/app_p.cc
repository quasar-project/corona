#include <corona-standalone/app/app_p.hh>

#include <qicon.h>
#include <qfileinfo.h>
#include <qqmlapplicationengine.h>
#include <corona-standalone/app/default_themes.hh>
#include <corona-standalone/gui/theme/qml/class_theme_wrapper.hh>
#include <corona-standalone/gui/immediate/terminal_commands.hh>

namespace corona::standalone::app
{
  auto detail::platform_dependent_icon(std::string_view stem) -> ::QIcon {
    if constexpr(fl::current_platform.operating_system == fl::platform::operating_system::windows)
      return ::QIcon(::QString::fromStdString(":/" + std::string(stem) + ".ico"));
    return ::QIcon(::QString::fromStdString(":/" + std::string(stem) + ".png"));
  }

  auto detail::qml_url(std::string_view path) -> ::QUrl {
    return { ::QString::fromStdString(fmt::format("qrc:/{}.qml", path)) };
  }

  ImGUIData::ImGUIData(CLogger& logger)
    : terminal_cmd(std::make_unique<gui::immediate::custom_command_struct>())
    , terminal(std::make_unique<extern_::imterm::terminal<gui::immediate::terminal_commands>>(*this->terminal_cmd, "Debug console"))
  {
    this->terminal->get_terminal_helper()->set_pattern("[%X] (%n) [%^%l%$] %^%v%$");
    logger->sinks().push_back(this->terminal->get_terminal_helper());
    llog::trace("ImGUIData: added default logger to debug console");
    CUserInterfaceLogger::ref_mut().logger().sinks().push_back(this->terminal->get_terminal_helper());
    llog::trace("ImGUIData: added ui logger to debug console");
    this->terminal->execute("configure_terminal colors set-theme \"Dark Cherry\"");
  }

  Corona::impl::impl(CLogger& logger)
    : logger(logger)
    , imgui(logger)
    , theme(fl::make_box<gui::theme::qml::CThemeWrapper>(nullptr))
    , app_dirs(fl::make_box<qml::CApplicationDirsWrapper>(corona::meta::corona_meta, nullptr))
    , map_view_manager(fl::make_box<map::CMapViewManager>(**this->app_dirs, nullptr))
  {
    llog::info("app: {}", corona::standalone::app::meta::corona_meta);
    llog::info("lib: {}", corona::meta::corona_meta);
  }

  Corona::impl::~impl() = default;

  auto Corona::impl::emplace_themes() -> void {
    llog::debug("Corona: emplacing application themes");
    for(auto const& path : default_themes) {
      auto const from = std::filesystem::path(path);
      auto const stem = ::QFileInfo(path.data()).fileName().toStdString();
      auto const to = this->theme.ref_mut().unwrap().folder() / stem;
      if(::QFile::exists(to))
        continue;
      if(not ::QFile::copy(from, to))
        llog::warn("Corona: failed to copy {} to {}", from.string(), to.string());
      else
        llog::trace("Corona: emplaced theme \'{}\'", stem);
    }
  }

  auto Corona::impl::configure_imgui(::QQmlApplicationEngine* engine) -> void {
    llog::trace("Corona: configuring imgui");
    this->imgui.imgui = dynamic_cast<imgui_renderer::CExtendableRenderer*>(
      imgui_renderer::CExtendableRenderer::from_engine(engine)
    );
    this->imgui.imgui->style_default();
    *this->imgui.imgui += [this](){ this->imgui.terminal->show(); };
    llog::trace("Corona: imgui configured successfully");
  }
} // namespace corona::standalone::app
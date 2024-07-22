#include <corona-standalone/app/app.hh>

#include <qfile.h>
#include <qfileinfo.h>
#include <qqml.h>
#include <qqmlcontext.h>
#include <qqmlapplicationengine.h>
#include <qquickwindow.h>
#include <qquickstyle.h>
#include <magic_enum/magic_enum.hpp>
#include <floppy/directories.h>
#include <qdebugenv/class_extendable_renderer.h>
#include <corona-standalone/utility/formatters.hh>
#include <corona-standalone/app/ui_logger.hh>
#include <corona-standalone/app/default_themes.hh>
#include <corona-standalone/gui/theme/qml/theme_wrapper.hh>
#include <corona-standalone/gui/immediate/terminal_commands.hh>

namespace me = magic_enum;
namespace
{
  [[nodiscard]] inline auto platform_dependent_icon(std::string_view const stem) -> QIcon {
    if constexpr(fl::current_platform.operating_system == fl::platform::operating_system::windows)
      return QIcon(QString::fromStdString(":/" + std::string(stem) + ".ico"));
    return QIcon(QString::fromStdString(":/" + std::string(stem) + ".png"));
  }

  [[nodiscard]] inline auto qml_url(std::string_view const path) -> QUrl {
    return { QString::fromStdString(fmt::format("qrc:/{}.qml", path)) };
  }
} // namespace

namespace corona::standalone::app
{
  struct Corona::impl
  {
    explicit impl(Logger& logger);

    auto emplace_themes() -> void;

    Logger& logger;
    fl::filesystem::application_dirs dirs;
    fl::box<gui::theme::qml::ThemeWrapper> theme;
    qdebugenv::CExtendableRenderer* imgui{nullptr};
    std::unique_ptr<gui::immediate::custom_command_struct> terminal_cmd;
    std::unique_ptr<imterm::terminal<gui::immediate::terminal_commands>> terminal;
  };

  Corona::impl::impl(Logger& logger)
    : logger(logger)
    , dirs(fl::filesystem::application_dirs(corona::standalone::app::meta::corona_meta))
    , theme(fl::make_box<gui::theme::qml::ThemeWrapper>(nullptr))
    , terminal_cmd(std::make_unique<gui::immediate::custom_command_struct>())
    , terminal(std::make_unique<imterm::terminal<gui::immediate::terminal_commands>>(*this->terminal_cmd, "Debug console"))
  {
    this->logger->sinks().push_back(this->terminal->get_terminal_helper());
    llog::trace("added default logger to debug console");
    UILogger::ref_mut().logger().sinks().push_back(this->terminal->get_terminal_helper());
    llog::trace("added ui logger to debug console");
    this->terminal->execute("configure_terminal colors set-theme \"Dark Cherry\"");
    llog::info("app: {}", corona::standalone::app::meta::corona_meta);
    llog::info("lib: {}", corona::meta::corona_meta);
  }

  auto Corona::impl::emplace_themes() -> void {
    llog::debug("emplacing application themes");
    for(auto const& path : default_themes) {
      auto const from = std::filesystem::path(path);
      auto const stem = ::QFileInfo(path.data()).fileName().toStdString();
      auto const to = this->theme.ref_mut().unwrap().folder() / stem;
      if(::QFile::exists(to))
        continue;
      if(not ::QFile::copy(from, to))
        llog::warn("failed to copy {} to {}", from.string(), to.string());
      else
        llog::trace("emplaced theme \'{}\'", stem);
    }
  }

  Corona::Corona(int& args, char** argv, Logger& logger)
    : IApplication(args, argv)
    , impl_(fl::make_box<impl>(logger))
  {
    Corona::setApplicationName(corona::standalone::app::meta::corona_meta.name().data());
    Corona::setApplicationVersion(corona::standalone::app::meta::corona_meta.version().as_str().data());
    Corona::setOrganizationName(corona::standalone::app::meta::corona_meta.organization().data());
    Corona::setOrganizationDomain(corona::standalone::app::meta::corona_meta.domain().data());
    ::qInstallMessageHandler(UILogger::message_handler);

    this->impl_->emplace_themes();
    llog::debug("initialized {}", fl::source_location::current().function_name());

    ::qmlRegisterSingletonInstance("io.corona.standalone.app", 1, 0, "Theme", impl_->theme.ptr_mut());
  }

  Corona::~Corona() { spdlog::set_level(spdlog::level::off); }

  auto Corona::with_icon(string_view const path) -> Corona& {
    llog::trace("app icon is set to \'{}\'", path);
    Corona::setWindowIcon(::platform_dependent_icon(path));
    return *this;
  }

  auto Corona::with_quick_style(QuickStyle style) -> Corona& {
    if(style == QuickStyle::Material)
      ::qputenv("QT_QUICK_CONTROLS_MATERIAL_VARIANT", "Dense");
    ::QQuickStyle::setStyle(::QString::fromStdString(string(me::enum_name(style))));
    llog::trace("app quick style is set to \'{}\'", string(me::enum_name(style)));
    return *this;
  }

  auto Corona::run_scene(string_view path) -> int {
    llog::debug("preparing to run quick scene");
    auto const u = ::qml_url(path);
    llog::trace("qml root url is set to `{}`", u.toString());
    auto engine = ::QQmlApplicationEngine();
    ::QQuickWindow::setDefaultAlphaBuffer(true);
    llog::trace("created default qml engine");
    engine.load(u);
    if(engine.rootObjects().isEmpty())
      fl::panic("failed to load qml scene");
    llog::debug("loaded qml scene");
    ::QObject::connect(this, &Corona::aboutToQuit, [&engine] {
      llog::info("cleaning up and quitting");
      engine.quit();
    });
    this->impl_->imgui = dynamic_cast<qdebugenv::CExtendableRenderer*>(
      qdebugenv::CExtendableRenderer::from_engine(&engine)
    );
    this->imgui_mut().style_default();
    this->imgui_mut() += [this](){
      this->impl_->terminal->show();
    };
    return Corona::exec();
  }

  auto Corona::logger() const -> Logger const& { return this->impl_->logger; }
  auto Corona::logger_mut() -> Logger& { return this->impl_->logger; }
  auto Corona::dirs() const -> fl::filesystem::application_dirs const& { return this->impl_->dirs; }
  auto Corona::dirs_mut() -> fl::filesystem::application_dirs& { return this->impl_->dirs; }
  auto Corona::theme() const -> gui::theme::qml::ThemeWrapper const& { return *this->impl_->theme; }
  auto Corona::theme_mut() -> gui::theme::qml::ThemeWrapper& { return *this->impl_->theme; }
  auto Corona::imgui() const -> qdebugenv::CExtendableRenderer const& { return *this->impl_->imgui; }
  auto Corona::imgui_mut() -> qdebugenv::CExtendableRenderer& { return *this->impl_->imgui; }
} // namespace corona::standalone::app
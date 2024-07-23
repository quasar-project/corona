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
#include <corona-standalone/app/class_user_interface_logger.hh>
#include <corona-standalone/app/default_themes.hh>
#include <corona-standalone/gui/theme/qml/class_theme_wrapper.hh>
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
  struct ImGUIData
  {
    explicit ImGUIData(CLogger& logger)
      : terminal_cmd(std::make_unique<gui::immediate::custom_command_struct>())
      , terminal(std::make_unique<imterm::terminal<gui::immediate::terminal_commands>>(*this->terminal_cmd, "Debug console"))
    {
      logger->sinks().push_back(this->terminal->get_terminal_helper());
      llog::trace("added default logger to debug console");
      CUserInterfaceLogger::ref_mut().logger().sinks().push_back(this->terminal->get_terminal_helper());
      llog::trace("added ui logger to debug console");
      this->terminal->execute("configure_terminal colors set-theme \"Dark Cherry\"");
    }

    qdebugenv::CExtendableRenderer* imgui{nullptr};
    std::unique_ptr<gui::immediate::custom_command_struct> terminal_cmd;
    std::unique_ptr<imterm::terminal<gui::immediate::terminal_commands>> terminal;
  };

  struct Corona::impl
  {
    explicit impl(CLogger& logger);

    auto emplace_themes() -> void;
    auto configure_imgui(::QQmlApplicationEngine* engine) -> void;

    CLogger& logger;
    fl::filesystem::application_dirs dirs;
    fl::box<gui::theme::qml::CThemeWrapper> theme;
    ImGUIData imgui;
  };

  Corona::impl::impl(CLogger& logger)
    : logger(logger)
    , dirs(fl::filesystem::application_dirs(corona::standalone::app::meta::corona_meta))
    , theme(fl::make_box<gui::theme::qml::CThemeWrapper>(nullptr))
    , imgui(logger)
  {
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

  auto Corona::impl::configure_imgui(::QQmlApplicationEngine* engine) -> void {
    llog::trace("configuring imgui");
    this->imgui.imgui = dynamic_cast<qdebugenv::CExtendableRenderer*>(
      qdebugenv::CExtendableRenderer::from_engine(engine)
    );
    this->imgui.imgui->style_default();
    *this->imgui.imgui += [this](){ this->imgui.terminal->show(); };
    llog::trace("imgui configured successfully");
  }

  Corona::Corona(int& args, char** argv, CLogger& logger)
    : IApplication(args, argv)
    , impl_(fl::make_box<impl>(logger))
  {
    llog::debug("Corona::Corona: running initialization");
    Corona::setApplicationName(corona::standalone::app::meta::corona_meta.name().data());
    Corona::setApplicationVersion(corona::standalone::app::meta::corona_meta.version().as_str().data());
    Corona::setOrganizationName(corona::standalone::app::meta::corona_meta.organization().data());
    Corona::setOrganizationDomain(corona::standalone::app::meta::corona_meta.domain().data());
    CUserInterfaceLogger::ref_mut().install();

    this->impl_->emplace_themes();
    llog::debug("initialized {}", fl::source_location::current().function_name());

    ::qmlRegisterSingletonInstance("io.corona.standalone.app", 1, 0, "Theme", impl_->theme.ptr_mut());
    llog::debug("Corona::Corona: initialization complete");
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
    this->impl_->configure_imgui(&engine);
    return Corona::exec();
  }

  auto Corona::logger() const -> CLogger const& { return this->impl_->logger; }
  auto Corona::logger_mut() -> CLogger& { return this->impl_->logger; }
  auto Corona::dirs() const -> fl::filesystem::application_dirs const& { return this->impl_->dirs; }
  auto Corona::dirs_mut() -> fl::filesystem::application_dirs& { return this->impl_->dirs; }
  auto Corona::theme() const -> gui::theme::qml::CThemeWrapper const& { return *this->impl_->theme; }
  auto Corona::theme_mut() -> gui::theme::qml::CThemeWrapper& { return *this->impl_->theme; }
  auto Corona::imgui() const -> qdebugenv::CExtendableRenderer const& { return *this->impl_->imgui.imgui; }
  auto Corona::imgui_mut() -> qdebugenv::CExtendableRenderer& { return *this->impl_->imgui.imgui; }
} // namespace corona::standalone::app
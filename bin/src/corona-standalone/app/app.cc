#include <corona-standalone/app/app.hh>

#include <qqml.h>
#include <qqmlcomponent.h>
#include <qqmlcontext.h>
#include <qqmlapplicationengine.h>
#include <qquickwindow.h>
#include <qquickstyle.h>
#include <magic_enum/magic_enum.hpp>
#include <floppy/directories.h>
#include <corona-standalone/utility/formatters.hh>
#include <corona-standalone/app/ui_logger.hh>
#include <corona-standalone/gui/theme/qml/theme_wrapper.hh>

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
    impl();

    fl::filesystem::application_dirs dirs;
    fl::box<gui::theme::qml::ThemeWrapper> theme;
  };

  Corona::impl::impl()
    : dirs(fl::filesystem::application_dirs(corona::standalone::app::meta::corona_meta))
    , theme(fl::make_box<gui::theme::qml::ThemeWrapper>(nullptr))
  {}

  Corona::Corona(int& args, char** argv)
    : IApplication(args, argv)
    , impl_(fl::make_box<impl>())
  {
    llog::debug()("initialized {}", fl::source_location::current().function_name());
    llog::info()("app: {}", corona::standalone::app::meta::corona_meta);
    llog::info()("lib: {}", corona::meta::corona_meta);
    Corona::setApplicationName(corona::standalone::app::meta::corona_meta.name().data());
    Corona::setApplicationVersion(corona::standalone::app::meta::corona_meta.version().as_str().data());
    Corona::setOrganizationName(corona::standalone::app::meta::corona_meta.organization().data());
    Corona::setOrganizationDomain(corona::standalone::app::meta::corona_meta.domain().data());
    ::qInstallMessageHandler(UILogger::message_handler);

    ::qmlRegisterSingletonInstance("io.corona.standalone.app", 1, 0, "Theme", impl_->theme.ptr_mut());
  }
  auto Corona::with_icon(string_view const path) -> Corona& {
    llog::trace()("app icon is set to \'{}\'", path);
    Corona::setWindowIcon(::platform_dependent_icon(path));
    return *this;
  }

  auto Corona::with_quick_style(QuickStyle style) -> Corona& {
    if(style == QuickStyle::Material)
      ::qputenv("QT_QUICK_CONTROLS_MATERIAL_VARIANT", "Dense");
    ::QQuickStyle::setStyle(::QString::fromStdString(string(me::enum_name(style))));
    llog::trace()("app quick style is set to \'{}\'", string(me::enum_name(style)));
    return *this;
  }

  auto Corona::run_scene(string_view path) -> int {
    llog::debug()("preparing to run quick scene");
    auto const u = ::qml_url(path);
    llog::trace()("qml root url is set to `{}`", u.toString());
    auto engine = ::QQmlApplicationEngine();
    ::QQuickWindow::setDefaultAlphaBuffer(true);
    llog::trace()("created default qml engine");
    engine.load(u);
    if(engine.rootObjects().isEmpty())
      fl::panic("failed to load qml scene");
    llog::debug()("loaded qml scene");
    ::QObject::connect(this, &Corona::aboutToQuit, [&engine] {
      llog::info()("cleaning up and quitting");
      engine.quit();
    });
    return Corona::exec();
  }

  Corona::~Corona() = default;
} // namespace corona::standalone::app
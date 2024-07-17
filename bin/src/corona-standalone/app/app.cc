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
#include <corona-standalone/utility/formatters.hh>
#include <corona-standalone/app/ui_logger.hh>
#include <corona-standalone/app/default_themes.hh>
#include <corona-standalone/gui/theme/qml/theme_wrapper.hh>
#include <corona-standalone/gui/immediate/generic.hh>
#include <corona-standalone/gui/immediate/sink.hh>

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

    auto emplace_themes() -> void;

    fl::filesystem::application_dirs dirs;
    fl::box<gui::theme::qml::ThemeWrapper> theme;
    gui::immediate::GenericItem* imgui{nullptr};
    std::shared_ptr<gui::immediate::LogSink> imgui_sink{nullptr};
    QQuickWindow* quick_window{nullptr};
  };

  Corona::impl::impl()
    : dirs(fl::filesystem::application_dirs(corona::standalone::app::meta::corona_meta))
    , theme(fl::make_box<gui::theme::qml::ThemeWrapper>(nullptr))
  {}

  auto Corona::impl::emplace_themes() -> void {
    llog::debug()("emplacing application themes");
    for(auto const& path : default_themes) {
      auto const stem = ::QFileInfo(path.data()).fileName().toStdString();
      auto const target = (this->theme.ref_mut().unwrap().folder() / stem).string();
      if(::QFile::exists(target.data()))
        continue;
      if(not ::QFile::copy(path.data(), target.data()))
        llog::error()("failed to copy {} to {}", path.data(), (this->theme.ref_mut().unwrap().folder() / stem).string().data());
      else
        llog::trace()("emplaced theme \'{}\'", stem);
    }
  }

  Corona::Corona(int& args, char** argv)
    : IApplication(args, argv)
    , impl_(fl::make_box<impl>())
  {
    Corona::setApplicationName(corona::standalone::app::meta::corona_meta.name().data());
    Corona::setApplicationVersion(corona::standalone::app::meta::corona_meta.version().as_str().data());
    Corona::setOrganizationName(corona::standalone::app::meta::corona_meta.organization().data());
    Corona::setOrganizationDomain(corona::standalone::app::meta::corona_meta.domain().data());
    ::qInstallMessageHandler(UILogger::message_handler);

    this->impl_->emplace_themes();
    this->impl_->imgui_sink = gui::immediate::LogSink::create();
    spdlog::default_logger()->sinks().push_back(this->impl_->imgui_sink);
    llog::debug()("initialized {}", fl::source_location::current().function_name());
    llog::info()("app: {}", corona::standalone::app::meta::corona_meta);
    llog::info()("lib: {}", corona::meta::corona_meta);

    ::qmlRegisterSingletonInstance("io.corona.standalone.app", 1, 0, "Theme", impl_->theme.ptr_mut());
  }

  Corona::~Corona() = default;

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
    auto& io = im::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    im::ext::style(im::ext::default_palette, im::ext::style::roundings{});
    this->impl_->quick_window = qobject_cast<::QQuickWindow*>(engine.rootObjects().front());
    auto* imgui_ptr = this->quick_window()->findChild<gui::immediate::GenericItem*>("imgui");
    if(imgui_ptr == nullptr)
      fl::panic("failed to find imgui in qml scene");
    this->impl_->imgui = imgui_ptr;
    llog::trace()("found imgui local pointer: {}", static_cast<void*>(imgui_ptr));
    this->imgui_mut() += this->impl_->imgui_sink.get();
    return Corona::exec();
  }

  auto Corona::dirs() const -> fl::filesystem::application_dirs const& { return this->impl_->dirs; }
  auto Corona::dirs_mut() -> fl::filesystem::application_dirs& { return this->impl_->dirs; }
  auto Corona::theme() const -> gui::theme::qml::ThemeWrapper const& { return *this->impl_->theme; }
  auto Corona::theme_mut() -> gui::theme::qml::ThemeWrapper& { return *this->impl_->theme; }
  auto Corona::imgui() const -> gui::immediate::GenericItem const& { return *this->impl_->imgui; }
  auto Corona::imgui_mut() -> gui::immediate::GenericItem& { return *this->impl_->imgui; }
  auto Corona::quick_window() const -> QQuickWindow* { return this->impl_->quick_window; }
} // namespace corona::standalone::app
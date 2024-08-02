#include <corona-standalone/app/app.hh>
#include <corona-standalone/app/app_p.hh>

#include <qfile.h>
#include <qqmlcontext.h>
#include <qquickwindow.h>
#include <qquickstyle.h>
#include <qqmlapplicationengine.h>
#include <magic_enum/magic_enum.hpp>
#include <floppy/directories.h>
#include <corona/modules/geoservice/import.h>

namespace me = magic_enum;
namespace corona::standalone::app
{
  Corona::Corona(int& args, char** argv, CLogger& logger)
    : IApplication(args, argv)
    , impl_(fl::make_box<impl>(logger))
  {
    llog::debug("Corona: running initialization");
    Corona::setApplicationName(corona::standalone::app::meta::corona_meta.name().data());
    Corona::setApplicationVersion(corona::standalone::app::meta::corona_meta.version().as_str().data());
    Corona::setOrganizationName(corona::standalone::app::meta::corona_meta.organization().data());
    Corona::setOrganizationDomain(corona::standalone::app::meta::corona_meta.domain().data());
    CUserInterfaceLogger::ref_mut().install();

    this->impl_->emplace_themes();
    llog::debug("Corona: initialized {}", fl::source_location::current().function_name());
    this->impl_->register_qml_singletons();
    llog::debug("Corona: initialization complete");
  }

  Corona::~Corona() { spdlog::set_level(spdlog::level::off); }

  auto Corona::with_icon(string_view const path) -> Corona& {
    llog::trace("Corona: app icon is set to \'{}\'", path);
    Corona::setWindowIcon(detail::platform_dependent_icon(path));
    return *this;
  }

  auto Corona::with_quick_style(QuickStyle style) -> Corona& {
    if(style == QuickStyle::Material)
      ::qputenv("QT_QUICK_CONTROLS_MATERIAL_VARIANT", "Dense");
    ::QQuickStyle::setStyle(::QString::fromStdString(string(me::enum_name(style))));
    llog::trace("Corona: app quick style is set to \'{}\'", string(me::enum_name(style)));
    return *this;
  }

  auto Corona::run_scene(string_view path) -> int {
    Corona::load_plugins();
    llog::debug("Corona: preparing to run quick scene");
    auto const u = detail::qml_url(path);
    llog::trace("Corona: qml root url is set to `{}`", u.toString());
    auto engine = ::QQmlApplicationEngine();
    ::QQuickWindow::setDefaultAlphaBuffer(true);
    llog::trace("Corona: created default qml engine");
    engine.load(u);
    if(engine.rootObjects().isEmpty())
      fl::panic("Corona: failed to load qml scene");
    llog::debug("Corona: loaded qml scene");
    ::QObject::connect(this, &Corona::aboutToQuit, [&engine] {
      llog::info("Corona: cleaning up and quitting");
      engine.quit();
    });
    this->impl_->configure_imgui(&engine);
    return Corona::exec();
  }

  auto Corona::load_plugins() -> void {
    llog::debug("Corona: loading plugins");
    if(not modules::geoservice::import_plugin())
      fl::panic("Corona: failed to load geoservice plugin");
  }

  auto Corona::logger() const -> CLogger const& { return this->impl_->logger; }
  auto Corona::logger_mut() -> CLogger& { return this->impl_->logger; }
  auto Corona::dirs() const -> fl::filesystem::application_dirs const& { return **this->impl_->app_dirs; }
  auto Corona::dirs_mut() -> fl::filesystem::application_dirs& { return **this->impl_->app_dirs; }
  auto Corona::theme() const -> gui::theme::qml::CThemeWrapper const& { return *this->impl_->theme; }
  auto Corona::theme_mut() -> gui::theme::qml::CThemeWrapper& { return *this->impl_->theme; }
  auto Corona::imgui() const -> modules::imgui_renderer::CExtendableRenderer const& { return *this->impl_->imgui.imgui; }
  auto Corona::imgui_mut() -> modules::imgui_renderer::CExtendableRenderer& { return *this->impl_->imgui.imgui; }
} // namespace corona::standalone::app
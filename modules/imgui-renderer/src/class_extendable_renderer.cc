#include <corona/modules/imgui_renderer/class_extendable_renderer.h>

#include <qqmlregistration.h>

[[maybe_unused]] auto init_resources() -> void {
  Q_INIT_RESOURCE(qdebugenv_qml);
}

namespace corona::modules::imgui_renderer
{
  namespace qml
  {
    [[maybe_unused]] volatile auto register_1_ = []() { // NOLINT(*-identifier-naming, *-avoid-non-const-global-variables)
      ::init_resources();
      fmt::println("registering {} qml types", meta::qml_namespace_rendering);
      ::qmlRegisterModule(meta::qml_namespace_rendering, 1, 0);
      ::qmlRegisterType<CGenericRenderer>(meta::qml_namespace_rendering, 1, 0, "GenericRenderer");
      ::qmlRegisterType<CExtendableRenderer>(meta::qml_namespace_rendering, 1, 0, "ExtendableRenderer");
      ::qmlRegisterType(QUrl("qrc:/qdebugenv/ImmediateGUIRenderingFacility.qml"), meta::qml_namespace_rendering, 1, 0, "ImmediateGUIRenderingFacility");
      return true;
    }();
  } // namespace qml

  CExtendableRenderer::CExtendableRenderer(CGenericRenderer* parent) : CGenericRenderer(parent) {}
  CExtendableRenderer::~CExtendableRenderer() = default;

  auto CExtendableRenderer::frame() -> void {
    this->CGenericRenderer::frame();
    for(auto& callback : this->callbacks_)
      callback();
    for(auto& drawable : this->drawables_)
      drawable->frame();
  }
} // namespace corona::modules::imgui_renderer
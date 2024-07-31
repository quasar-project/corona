#include <qdebugenv/class_generic_renderer.h>

#include <qguiapplication.h>
#include <qquickwindow.h>
#include <qqmlapplicationengine.h>
#include <private/qquickgraphicsinfo_p.h>
#include <magic_enum/magic_enum.hpp>
#include <floppy/logging.h>
#include <qdebugenv/rhi/class_renderer.h>
#include <qdebugenv/rhi/class_immediate_gui_bridge.h>
#include <qdebugenv/vendored/imgui.h>
#include <qdebugenv/imgui/all.h>

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
# include <qsgrendernode.h>
#else // QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
# include <private/qsgrendernode_p.h>
#endif // QT_VERSION > QT_VERSION_CHECK(6, 6, 0)

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
# define QDE_WELL_BEHAVING_DEPTH 1
#endif // QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)

namespace me = magic_enum;
namespace llog = ::floppy::log;
namespace
{
  using namespace ::qdebugenv;
  class CRhiImguiNode : public ::QSGRenderNode
  {
   public:
    CRhiImguiNode(::QQuickWindow* window, CGenericRenderer* item)
      : window(window)
      , item(item)
      , renderer(new CRhiRenderer)
      , custom_renderer(this->item->create_custom_renderer())
    {}

    ~CRhiImguiNode() {
      delete this->renderer;
      delete this->custom_renderer;
    }

    virtual auto prepare() -> void override {
      #if QT_VERSION_MAJOR > 6 || QT_VERSION_MINOR >= 6
      auto* rhi = this->window->rhi();
      #else // QT_VERSION_MAJOR > 6 || QT_VERSION_MINOR >= 6
      auto* rif = this->window->rendererInterface();
      auto* rhi = static_cast<::QRhi*>(rif->getResource(this->window, ::QSGRendererInterface::RhiResouce));
      #endif // QT_VERSION_MAJOR > 6 || QT_VERSION_MINOR >= 6
      if(not rhi) {
        llog::error("CRhiImguiNode::prepare: no rhi found for window {}", static_cast<void*>(this->window));
        return;
      }
      if(this->custom_renderer)
        this->custom_renderer->render();

      #if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
      auto* rt = this->renderTarget();
      auto* cb = this->commandBuffer();
      #else // QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
      auto* d = ::QSGRenderNodePrivate::get(this);
      auto* rt = d->m_rt.rt;
      auto* cb = d->m_rt.cb;
      #endif // QT_VERSION > QT_VERSION_CHECK(6, 6, 0)

      #if defined(QDE_WELL_BEHAVING_DEPTH)
      auto const mvp = *this->projectionMatrix() * *this->matrix();
      #else // defined(QDE_WELL_BEHAVING_DEPTH)
      auto mvp = rhi->clipSpaceCorrMatrix();
      auto const output_size = rt->pixelSize();
      auto const dpr = rt->devicePixelRatio();
      mvp.ortho(0, output_size.width() / dpr, output_size.height() / dpr, 0, -1, 1);
      mvp *= *this->matrix();
      #endif // defined(QDE_WELL_BEHAVING_DEPTH)

      auto const opacity = static_cast<f32>(this->inheritedOpacity());
      this->renderer->prepare(rhi, rt, cb, mvp, opacity);
    }

    virtual auto render([[maybe_unused]] ::QSGRenderNode::RenderState const* state) -> void override {
      this->renderer->render();
    }

    virtual auto releaseResources() -> void override {
      this->renderer->release_resources();
    }

    [[nodiscard]] virtual auto changedStates() const -> ::QSGRenderNode::StateFlags override {
      return DepthState | ScissorState | ColorState | BlendState | CullState | ViewportState;
    }

    [[nodiscard]] virtual auto flags() const -> ::QSGRenderNode::RenderingFlags override {
      auto result = ::QSGRenderNode::RenderingFlags(NoExternalRendering);

      #if defined(QDE_WELL_BEHAVING_DEPTH)
      result |= DepthAwareRendering;
      #endif // defined(QDE_WELL_BEHAVING_DEPTH)

      return result;
    }

    ::QQuickWindow* window;
    CGenericRenderer* item;
    CRhiRenderer* renderer;
    CRhiImmediateGuiCustomRenderer* custom_renderer = nullptr;
  };

  struct GraphicsInfo
  {
    std::string backend;
    std::string version;
    std::string profile;
    std::string renderable_type;
    std::string shader_compilation_type;
    std::string shader_source_type;
    std::string shader_type;

    [[nodiscard]] static auto from_graphics_info(::QQuickGraphicsInfo const& info) -> GraphicsInfo {
      static auto self = GraphicsInfo();
      self.backend = me::enum_name(info.api());
      self.version = fmt::format("{}.{}", info.majorVersion(), info.minorVersion());
      self.profile = me::enum_name(info.profile());
      self.renderable_type = me::enum_name(info.renderableType());
      self.shader_compilation_type = me::enum_name(info.shaderCompilationType());
      self.shader_source_type = me::enum_name(info.shaderSourceType());
      self.shader_type = me::enum_name(info.shaderType());
      return self;
    }
  };
} // namespace

namespace qdebugenv
{
  CRhiImmediateGuiCustomRenderer::~CRhiImmediateGuiCustomRenderer() = default;
  auto CRhiImmediateGuiCustomRenderer::sync([[maybe_unused]] CRhiRenderer* renderer) -> void {}
  auto CRhiImmediateGuiCustomRenderer::render() -> void {}

  struct CGenericRenderer::impl
  {
    CGenericRenderer* q;
    ::QQuickWindow* window = nullptr;
    ::QMetaObject::Connection window_connection;
    CImmediateGuiBridge gui;
    ::QQuickGraphicsInfo* graphics_info = nullptr;
    bool show_metrics = true;

    explicit impl(CGenericRenderer* q) : q(q) {}
  };

  CGenericRenderer::CGenericRenderer(::QQuickItem* parent)
    : QQuickItem(parent)
    , impl_(fl::make_box<impl>(this)) {
    this->setFlag(::QQuickItem::ItemHasContents, true);
    this->setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    this->setAcceptHoverEvents(true);
  }

  CGenericRenderer::~CGenericRenderer() = default;

  auto CGenericRenderer::frame() -> void {
    using enum imgui::palette::color;

    auto& io = ImGui::GetIO();
    ImGui::Begin("Scenegraph metrics", &this->impl_->show_metrics);
    if(this->impl_->graphics_info) {
      auto const info = ::GraphicsInfo::from_graphics_info(*this->impl_->graphics_info);
      imgui::debug_field(imgui::default_palette[green], "Backend", "{}", info.backend);
      imgui::debug_field(imgui::default_palette[purple], "Version", "{}", info.version);
      imgui::debug_field(imgui::default_palette[purple], "Profile", "{}", info.profile);
      imgui::debug_field(imgui::default_palette[comment], "Renderable type", "{}", info.renderable_type);
      imgui::debug_field(imgui::default_palette[comment], "Shader compilation type", "{}", info.shader_compilation_type);
      imgui::debug_field(imgui::default_palette[comment], "Shader source type", "{}", info.shader_source_type);
      imgui::debug_field(imgui::default_palette[purple], "Shader type", "{}", info.shader_type);
    } else
      imgui::text(imgui::default_palette[red], "No graphics info available");
    imgui::separator();
    imgui::debug_field(imgui::default_palette[comment], "Framerate", "{:.1f} FPS ({:.5f} ms/frame)", io.Framerate, 1'000.0F / io.Framerate);
    ImGui::End();
  }

  auto CGenericRenderer::create_custom_renderer() -> CRhiImmediateGuiCustomRenderer* {
    return nullptr;
  }

  auto CGenericRenderer::bridge() -> CImmediateGuiBridge* {
    return &this->impl_->gui;
  }

  auto CGenericRenderer::graphics_info() const -> QObject* {
    return this->impl_->graphics_info;
  }

  auto CGenericRenderer::set_graphics_info(QObject* info) -> void {
    if(this->impl_->graphics_info == info)
      return;
    this->impl_->graphics_info = static_cast<::QQuickGraphicsInfo*>(info);
    emit this->impl_->q->graphics_info_changed();
  }

  auto CGenericRenderer::updatePaintNode(
    ::QSGNode* node,
    [[maybe_unused]] ::QQuickItem::UpdatePaintNodeData* data
  ) -> ::QSGNode* {
    if(this->size().isEmpty()) {
      delete node; // NOLINT(*-owning-memory)
      return nullptr;
    }
    auto* n = dynamic_cast<CRhiImguiNode*>(node);
    if(not n)
      n = new CRhiImguiNode(this->impl_->window, this); // NOLINT(*-owning-memory)
    this->impl_->gui.sync_renderer(n->renderer);
    if(n->custom_renderer)
      n->custom_renderer->sync(n->renderer);
    n->markDirty(::QSGNode::DirtyMaterial);
    return n;
  }
  auto CGenericRenderer::itemChange(
    ::QQuickItem::ItemChange change,
    ::QQuickItem::ItemChangeData const& value) -> void {
    if(change != ::QQuickItem::ItemSceneChange)
      return;
    if(this->impl_->window) {
      ::QObject::disconnect(this->impl_->window_connection);
      this->impl_->window = nullptr;
    }
    if(value.window) { // NOLINT(*-pro-type-union-access)
      this->impl_->window = this->window();
      this->impl_->window_connection = QObject::connect(
        this->impl_->window,
        &::QQuickWindow::afterAnimating,
        this->impl_->window,
        [this] -> void {
          if(not this->isVisible())
            return;
          this->impl_->gui.next_frame(
            this->size(),
            static_cast<f32>(this->impl_->window->effectiveDevicePixelRatio()),
            this->mapToScene({0., 0.}),
            [this] -> void { this->frame(); }
          );
          this->update();
          if(not this->impl_->window->isSceneGraphInitialized())
            this->impl_->window->update();
        });
    }
  }
  auto CGenericRenderer::keyPressEvent(::QKeyEvent* event) -> void { this->impl_->gui.process_event(event); }
  auto CGenericRenderer::keyReleaseEvent(::QKeyEvent* event) -> void { this->impl_->gui.process_event(event); }
  auto CGenericRenderer::mousePressEvent(::QMouseEvent* event) -> void {
    this->forceActiveFocus(Qt::MouseFocusReason);
    this->impl_->gui.process_event(event);
  }
  auto CGenericRenderer::mouseReleaseEvent(::QMouseEvent* event) -> void { this->impl_->gui.process_event(event); }
  auto CGenericRenderer::mouseMoveEvent(::QMouseEvent* event) -> void { this->impl_->gui.process_event(event); }
  auto CGenericRenderer::mouseDoubleClickEvent(::QMouseEvent* event) -> void { this->impl_->gui.process_event(event); }
  auto CGenericRenderer::wheelEvent(::QWheelEvent* event) -> void { this->impl_->gui.process_event(event); }
  auto CGenericRenderer::hoverMoveEvent(::QHoverEvent* event) -> void {
    if(::QGuiApplication::mouseButtons() != Qt::NoButton)
      return;
    auto const scene_offset = this->mapToScene(event->position());
    auto const global_offset = this->mapToGlobal(scene_offset);
    auto e = ::QMouseEvent(
      ::QEvent::MouseMove,
      event->position(),
      event->position() + scene_offset,
      event->position() + global_offset,
      Qt::NoButton,
      Qt::NoButton,
      ::QGuiApplication::keyboardModifiers()
    );
    this->impl_->gui.process_event(&e);
  }
  auto CGenericRenderer::touchEvent(::QTouchEvent* event) -> void { this->impl_->gui.process_event(event); }
  auto CGenericRenderer::from_window(::QQuickWindow* window) -> CGenericRenderer* {
    return window->findChild<CGenericRenderer*>("imgui");
  }
  auto CGenericRenderer::from_engine(::QQmlApplicationEngine* engine) -> CGenericRenderer* {
    return CGenericRenderer::from_window(qobject_cast<::QQuickWindow*>(engine->rootObjects().front()));
  }

  auto CGenericRenderer::style_default() -> void {
    [[maybe_unused]] auto _ = imgui::style(imgui::default_palette, imgui::style::roundings());
  }
} // namespace qdebugenv
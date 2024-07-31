#pragma once

#include <qquickitem.h>
#include <qdebugenv/detail/export.h>
#include <qdebugenv/rhi/class_immediate_gui_custom_renderer.h>

class QQuickWindow;
class QQmlApplicationEngine;
namespace qdebugenv
{
  class CRhiRenderer;
  class CImmediateGuiBridge;

  class QDE_API CGenericRenderer : public ::QQuickItem
  {
    Q_OBJECT
    Q_PROPERTY(QObject* graphicsInfo READ graphics_info WRITE set_graphics_info NOTIFY graphics_info_changed)

   public:
    explicit CGenericRenderer(::QQuickItem* parent = nullptr);
    virtual ~CGenericRenderer() override;
    CGenericRenderer(const CGenericRenderer&) = delete;
    CGenericRenderer(CGenericRenderer&&) = delete;
    auto operator=(const CGenericRenderer&) -> CGenericRenderer& = delete;
    auto operator=(CGenericRenderer&&) -> CGenericRenderer& = delete;

    virtual auto style_default() -> void;
    virtual auto frame() -> void;
    [[nodiscard]] virtual auto create_custom_renderer() -> CRhiImmediateGuiCustomRenderer*;

    [[nodiscard]] auto bridge() -> CImmediateGuiBridge*;
    [[nodiscard]] auto graphics_info() const -> QObject*;
    auto set_graphics_info(QObject* info) -> void;

    [[nodiscard]] static auto from_window(::QQuickWindow* window) -> CGenericRenderer*;
    [[nodiscard]] static auto from_engine(::QQmlApplicationEngine* engine) -> CGenericRenderer*;

   signals:
      void graphics_info_changed();

   private:
    [[nodiscard]] virtual auto updatePaintNode(
      ::QSGNode* node,
      ::QQuickItem::UpdatePaintNodeData* data
    ) -> ::QSGNode* override;

    virtual auto itemChange(
      ::QQuickItem::ItemChange change,
      ::QQuickItem::ItemChangeData const& value
    ) -> void override;

    virtual auto keyPressEvent(::QKeyEvent* event) -> void override;
    virtual auto keyReleaseEvent(::QKeyEvent* event) -> void override;
    virtual auto mousePressEvent(::QMouseEvent* event) -> void override;
    virtual auto mouseReleaseEvent(::QMouseEvent* event) -> void override;
    virtual auto mouseMoveEvent(::QMouseEvent* event) -> void override;
    virtual auto mouseDoubleClickEvent(::QMouseEvent* event) -> void override;
    virtual auto wheelEvent(::QWheelEvent* event) -> void override;
    virtual auto hoverMoveEvent(::QHoverEvent* event) -> void override;
    virtual auto touchEvent(::QTouchEvent* event) -> void override;

    struct impl;
    fl::traits::pimpl<impl> impl_;
  };
} // namespace qdebugenv
#pragma once

#include <qpropertyanimation.h>
#include <qpainter.h>
#include <qquickpainteditem.h>
#include <corona-standalone/definitions.hh>

namespace corona::standalone::gui::theme
{
  class CircularReveal : public QQuickPaintedItem
  {
    Q_OBJECT
    Q_PROPERTY(QQuickItem* target READ target WRITE setTarget NOTIFY targetChanged FINAL REQUIRED)
    Q_PROPERTY(int radius READ radius WRITE setRadius NOTIFY radiusChanged FINAL)

   public:
    explicit CircularReveal(::QQuickItem* parent = nullptr);

    [[nodiscard]] auto target() const -> ::QQuickItem*;
    [[nodiscard]] auto radius() const -> int;

    auto setTarget(::QQuickItem*) -> void; // NOLINT(*-identifier-naming)
    auto setRadius(int) -> void; // NOLINT(*-identifier-naming)

    virtual auto paint(::QPainter* painter) -> void override; // NOLINT(*-use-override)
    Q_INVOKABLE void start(int w, int h, ::QPoint const& center, int radius) noexcept;
    Q_SLOT void handleGrabResult(); // NOLINT(*-identifier-naming)

   signals:
    void targetChanged(); // NOLINT(*-identifier-naming)
    void radiusChanged(); // NOLINT(*-identifier-naming)
    void imageChanged(); // NOLINT(*-identifier-naming)
    void animationFinished(); // NOLINT(*-identifier-naming)

   private:
    ::QQuickItem* target_;
    int radius_;
    ::QImage source_;
    ::QPropertyAnimation anim_;
    ::QPoint center_;
    ::QSharedPointer<::QQuickItemGrabResult> grab_result_;
  };
} // namespace corona::standalone::gui::theme
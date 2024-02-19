/*
 * @BUG: Segfaults sometimes in QQuickItemGrabResult::render() method
 */

#pragma once

#include <QtCore/QPropertyAnimation>
#include <QtGui/QPainter>
#include <QtQuick/QQuickPaintedItem>

namespace gui::theme
{
  // ReSharper disable once CppClassCanBeFinal
  class CircularReveal : public QQuickPaintedItem
  {
    Q_OBJECT
    Q_PROPERTY(QQuickItem* target READ target WRITE setTarget NOTIFY targetChanged FINAL REQUIRED)
    Q_PROPERTY(int radius READ radius WRITE setRadius NOTIFY radiusChanged FINAL)

    public:
      explicit CircularReveal(QQuickItem* parent = nullptr);

      [[nodiscard]] QQuickItem* target() const;       void setTarget(QQuickItem*);
      [[nodiscard]] int radius() const;               void setRadius(int);

      virtual void paint(QPainter* painter) override;

      Q_INVOKABLE void start(int w, int h, const QPoint& center, int radius) noexcept;
      Q_SLOT void handleGrabResult();

      signals:
        void targetChanged();
        void radiusChanged();
        void imageChanged();
        void animationFinished();

    private:
      QQuickItem* m_target;
      int m_radius;
      QImage m_source;
      QPropertyAnimation m_anim;
      QPoint m_center;
      QSharedPointer<QQuickItemGrabResult> m_grab_result;
  };
} // QtEx
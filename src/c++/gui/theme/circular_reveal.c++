//
// Created by user on 05.01.2024.
//

#include "circular_reveal.h"
#include <QtGui/QGuiApplication>
#include <QtGui/QPainterPath>
#include <QtQuick/QQuickItemGrabResult>

namespace gui::theme
{
  CircularReveal::CircularReveal(QQuickItem* parent)
    : QQuickPaintedItem(parent)
    , m_anim(QPropertyAnimation(this, "radius", this))
  {
    this->setFlag(QQuickItem::ItemHasContents);
    this->setVisible(false);
    m_anim.setDuration(333);
    m_anim.setEasingCurve(QEasingCurve::OutCubic);
    Qt::Object::connect(&m_anim, &QPropertyAnimation::finished, this, [=, this](){
      this->update();
      this->setVisible(false);
      emit animationFinished();
    });
    Qt::Object::connect(this, &CircularReveal::radiusChanged, this, [=, this](){
      this->update();
    });
  }

  QQuickItem* CircularReveal::target() const { return m_target; }\
  void CircularReveal::setTarget(QQuickItem* x) {
    m_target = x;
    emit targetChanged();
  }

  int CircularReveal::radius() const { return m_radius; }
  void CircularReveal::setRadius(int x) {
    m_radius = x;
    emit radiusChanged();
  }

  void CircularReveal::paint(QPainter* painter)
  {
    painter->save();
    painter->drawImage(QRect(0, 0, static_cast<int>(width()), static_cast<int>(height())), m_source);
    QPainterPath path;
    path.moveTo(m_center.x(), m_center.y());
    path.addEllipse(QPointF(m_center.x(), m_center.y()), radius(), radius());
    painter->setCompositionMode(QPainter::CompositionMode_Clear);
    painter->fillPath(path, Qt::black);
    painter->restore();
  }

  void CircularReveal::start(int w, int h, const QPoint& center, int radius) noexcept
  {
    m_anim.setStartValue(0);
    m_anim.setEndValue(radius);
    m_center = center;
    m_grab_result = target()->grabToImage(QSize(w, h));
    Qt::Object::connect(m_grab_result.data(), &QQuickItemGrabResult::ready, this, &CircularReveal::handleGrabResult);
  }

  void CircularReveal::handleGrabResult()
  {
    m_grab_result.data()->image().swap(m_source);
    this->update();
    this->setVisible(true);
    emit imageChanged();
    m_anim.start();
  }
} // QtEx
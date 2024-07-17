#include <corona-standalone/gui/theme/reveal.hh>

#include <qguiapplication.h>
#include <qpainterpath.h>
#include <qquickitemgrabresult.h>

namespace corona::standalone::gui::theme
{
  CircularReveal::CircularReveal(::QQuickItem* parent)
    : ::QQuickPaintedItem(parent)
    , target_(nullptr)
    , radius_(0)
    , anim_(this, "radius", this)
    , center_(0, 0)
  {
    this->setFlag(ItemHasContents);
    this->setVisible(false);
    this->anim_.setDuration(333);
    this->anim_.setEasingCurve(::QEasingCurve::OutCubic);
    ::QObject::connect(&(this->anim_), &::QPropertyAnimation::finished, this, [this]() {
      this->update();
      this->setVisible(false);
      emit this->animationFinished();
    });
    ::QObject::connect(this, &CircularReveal::radiusChanged, this, [this]() { this->update(); });
  }
  auto CircularReveal::target() const -> ::QQuickItem* { return this->target_; }
  auto CircularReveal::setTarget(::QQuickItem* target) -> void {
    this->target_ = target;
    emit this->targetChanged();
  }

  auto CircularReveal::radius() const -> int { return this->radius_; }
  auto CircularReveal::setRadius(int radius) -> void {
    this->radius_ = radius;
    emit this->radiusChanged();
  }

  auto CircularReveal::paint(::QPainter* painter) -> void {
    painter->save();
    painter->drawImage(
      ::QRect(0, 0, static_cast<int>(this->width()), static_cast<int>(this->height())),
      this->source_
    );
    auto path = ::QPainterPath();
    path.addEllipse(this->center_, this->radius_, this->radius_);
    painter->setCompositionMode(QPainter::CompositionMode_Clear);
    painter->fillPath(path, Qt::black);
    painter->restore();
  }

  auto CircularReveal::start(int w, int h, ::QPoint const& center, int radius) noexcept -> void {
    this->anim_.setStartValue(0);
    this->anim_.setEndValue(radius);
    this->center_ = center;
    this->grab_result_ = this->target_->grabToImage(::QSize(w, h));
    ::QObject::connect(this->grab_result_.data(), &::QQuickItemGrabResult::ready, this, &CircularReveal::handleGrabResult);
  }

  auto CircularReveal::handleGrabResult() -> void {
    if(this->grab_result_.data() == nullptr)
      return;
    this->grab_result_.data()->image().swap(this->source_);
    this->update();
    this->setVisible(true);
    emit this->imageChanged();
    this->anim_.start();
  }
} // namespace corona::standalone::gui::theme
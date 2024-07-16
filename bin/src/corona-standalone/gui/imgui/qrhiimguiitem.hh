#pragma once

#include <qquickitem.h>

QT_BEGIN_NAMESPACE

struct QRhiImguiItemPrivate;
class QRhiImgui;
class QRhiImguiRenderer;

class QRhiImguiItemCustomRenderer
{
 public:
  virtual ~QRhiImguiItemCustomRenderer();
  virtual void sync(QRhiImguiRenderer *renderer);
  virtual void render();
};

class QRhiImguiItem : public QQuickItem
{
  Q_OBJECT

 public:
  QRhiImguiItem(QQuickItem *parent = nullptr);
  ~QRhiImguiItem();

  virtual void frame();
  virtual QRhiImguiItemCustomRenderer *createCustomRenderer();

  QRhiImgui* imgui();

 private:
  QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;
  void itemChange(QQuickItem::ItemChange, const QQuickItem::ItemChangeData &) override;

  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void hoverMoveEvent(QHoverEvent *event) override;
  void touchEvent(QTouchEvent *event) override;

  QRhiImguiItemPrivate *d;
};

QT_END_NAMESPACE
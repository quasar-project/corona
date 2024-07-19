// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef QRHIIMGUIITEM_H
#define QRHIIMGUIITEM_H

#include <QtQuick/qquickitem.h>

QT_BEGIN_NAMESPACE

struct QRhiImguiItemPrivate;
class QRhiImgui;
class QRhiImguiRenderer;

class QRhiGraphicsInfo
{
  Q_GADGET
  Q_PROPERTY(QString api READ api WRITE setApi)
  Q_PROPERTY(int majorVer READ majorVer WRITE setMajorVer)
  Q_PROPERTY(int minorVer READ minorVer WRITE setMinorVer)
  Q_PROPERTY(QString profile READ profile WRITE setProfile)
  Q_PROPERTY(QString renderableType READ renderableType WRITE setRenderableType)
  Q_PROPERTY(QString shaderCompilationType READ shaderCompilationType WRITE setShaderCompilationType)
  Q_PROPERTY(QString shaderSourceType READ shaderSourceType WRITE setShaderSourceType)
  Q_PROPERTY(QString shaderType READ shaderType WRITE setShaderType)

 public:
  [[nodiscard]] auto api() const -> QString { return this->m_api; }
  auto setApi(QString const& api) -> void { this->m_api = api; }

  [[nodiscard]] auto majorVer() const -> int { return this->m_major_ver; }
  auto setMajorVer(int major_ver) -> void { this->m_major_ver = major_ver; }

  [[nodiscard]] auto minorVer() const -> int { return this->m_minor_ver; }
  auto setMinorVer(int minor_ver) -> void { this->m_minor_ver = minor_ver; }

  [[nodiscard]] auto profile() const -> QString { return this->m_profile; }
  auto setProfile(QString const& profile) -> void { this->m_profile = profile; }

  [[nodiscard]] auto renderableType() const -> QString { return this->m_renderable_type; }
  auto setRenderableType(QString const& renderable_type) -> void { this->m_renderable_type = renderable_type; }

  [[nodiscard]] auto shaderCompilationType() const -> QString { return this->m_shader_compilation_type; }
  auto setShaderCompilationType(QString const& shader_compilation_type) -> void { this->m_shader_compilation_type = shader_compilation_type; }

  [[nodiscard]] auto shaderSourceType() const -> QString { return this->m_shader_source_type; }
  auto setShaderSourceType(QString const& shader_source_type) -> void { this->m_shader_source_type = shader_source_type; }

  [[nodiscard]] auto shaderType() const -> QString { return this->m_shader_type; }
  auto setShaderType(QString const& shader_type) -> void { this->m_shader_type = shader_type; }

 private:
  QString m_api = "Unknown";
  int m_major_ver = 0;
  int m_minor_ver = 0;
  QString m_profile = "Unknown";
  QString m_renderable_type = "Unknown";
  QString m_shader_compilation_type = "Unknown";
  QString m_shader_source_type = "Unknown";
  QString m_shader_type = "Unknown";
};

class QRhiImguiItemCustomRenderer
{
public:
    virtual ~QRhiImguiItemCustomRenderer();
    virtual void sync(QRhiImguiRenderer *renderer);
    virtual void render();
};

Q_DECLARE_METATYPE(QRhiGraphicsInfo)

class QRhiImguiItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QRhiGraphicsInfo graphicsInfo READ graphicsInfo WRITE setGraphicsInfo NOTIFY graphicsInfoChanged)

public:
    QRhiImguiItem(QQuickItem *parent = nullptr);
    ~QRhiImguiItem();

    virtual void frame();
    virtual QRhiImguiItemCustomRenderer *createCustomRenderer();

    QRhiImgui *imgui();

    auto graphicsInfo() const -> QRhiGraphicsInfo const&;
    void setGraphicsInfo(QRhiGraphicsInfo const& graphics_info);

    signals:
        void graphicsInfoChanged();

private:
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;
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
    QRhiGraphicsInfo m_graphics_info;
};

QT_END_NAMESPACE

#endif

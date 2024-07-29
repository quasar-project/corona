#pragma once

#include <qobject.h>
#include <qgeoserviceproviderfactory.h>

QT_BEGIN_NAMESPACE

class QGeoServicePluginCGS : public QObject,
                             public QGeoServiceProviderFactory
{
  Q_OBJECT
  Q_INTERFACES(QGeoServiceProviderFactory)
  Q_PLUGIN_METADATA(IID "org.qt-project.qt.geoservice.serviceproviderfactory/6.0"
                    FILE "cgs_plugin.json")

 public:
  QGeoServicePluginCGS() = default;

  auto createMappingManagerEngine(
    QVariantMap const& parameters,
    QGeoServiceProvider::Error* error,
    QString* errorString
  ) const -> QGeoMappingManagerEngine* override;
};

QT_END_NAMESPACE
#pragma once

#include <qnetworkreply.h>
#include <private/qgeotiledmapreply_p.h>

namespace corona::modules::geoservice
{
  class CGeoTiledMapReply : public ::QGeoTiledMapReply
  {
    Q_OBJECT

   public:
    CGeoTiledMapReply(
      ::QNetworkReply* reply,
      ::QGeoTileSpec const& spec,
      ::QString const& image_format,
      ::QObject* parent = nullptr
    );

   private slots:
    void networkReplyFinished();
    void networkReplyError(::QNetworkReply::NetworkError error);
  };
} // namespace corona::modules::geoservice
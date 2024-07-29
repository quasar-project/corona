#pragma once

#include <qpointer.h>
#include <qnetworkaccessmanager.h>
#include <private/qgeotilefetcher_p.h>
#include <corona/bootstrap/geoservice/class_config.h>

namespace corona::bootstrap::geoservice
{
  class CGeoTileFetcherMap : public ::QGeoTileFetcher
  {
    Q_OBJECT

   public:
    explicit CGeoTileFetcherMap(CConfig* cfg, ::QVariantMap const& parameters, ::QGeoMappingManagerEngine* parent = nullptr);

   private:
    auto getTileImage(::QGeoTileSpec const& spec) -> ::QGeoTiledMapReply* override;
    [[nodiscard]] auto url(::QGeoTileSpec const& spec) const -> ::QString;
    auto storage_network_tile(::QGeoTiledMapReply* reply) -> void;

    ::QPointer<::QNetworkAccessManager> network_manager_;
    CConfig* cfg_;
  };
} // namespace corona::bootstrap::geoservice
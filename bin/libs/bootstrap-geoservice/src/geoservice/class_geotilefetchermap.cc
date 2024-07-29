#include <geoservice/class_geotilefetchermap.hh>

#include <filesystem>
#include <qdir.h>
#include <qurl.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkrequest.h>
#include <private/qgeotilespec_p.h>
#include <private/qgeotilefetcher_p_p.h>
#include <floppy/logging.h>
#include <geoservice/parameters.hh>
#include <geoservice/class_geotiledreplymap.hh>

namespace llog = floppy::log;
namespace corona::bootstrap::geoservice
{
  CGeoTileFetcherMap::CGeoTileFetcherMap(
    CConfig* cfg,
    [[maybe_unused]] ::QVariantMap const& parameters,
    ::QGeoMappingManagerEngine* parent
  )
    : ::QGeoTileFetcher(parent)
    , network_manager_(new ::QNetworkAccessManager(this))
    , cfg_(cfg)
  {}

  auto CGeoTileFetcherMap::getTileImage(QGeoTileSpec const& spec) -> ::QGeoTiledMapReply* {
    auto request = ::QNetworkRequest();
    // todo: maybe user agent?
    request.setRawHeader("Accept", "*/*");
    request.setUrl(::QUrl(this->url(spec)));
    auto* reply = this->network_manager_->get(request);
    auto* map_reply = new CGeoTiledMapReply(reply, spec, {});
    //::QObject::connect(map_reply, &CGeoTiledMapReply::finished, this, [map_reply, this](){ this->storage_network_tile(map_reply); });
    return map_reply;
  }

  auto CGeoTileFetcherMap::url(QGeoTileSpec const& spec) const -> ::QString {
    auto const fmt_offline = (*this->cfg_)[spec.mapId()].offline_url;
    auto const offline_path = fmt::format(
      fmt::runtime(fmt_offline),
      fmt::arg("x", spec.x()),
      fmt::arg("y", spec.y()),
      fmt::arg("z", spec.zoom())
    );
    if(exists(std::filesystem::path(offline_path)))
      return ::QUrl(::QString::fromStdString(offline_path)).isLocalFile()
        ? ::QString::fromStdString(offline_path)
        : ::QUrl::fromLocalFile(::QString::fromStdString(offline_path)).toString();

    auto const fmt_online = (*this->cfg_)[spec.mapId()].online_url;
    return ::QString::fromStdString(fmt::format(
      fmt::runtime(fmt_online),
      fmt::arg("x", spec.x()),
      fmt::arg("y", spec.y()),
      fmt::arg("z", spec.zoom())
    ));
  }

  auto CGeoTileFetcherMap::storage_network_tile(::QGeoTiledMapReply* reply) -> void {
    if(reply->error() != ::QGeoTiledMapReply::NoError)
      return;
    auto const fmt_offline = (*this->cfg_)[reply->tileSpec().mapId()].offline_url;
    auto const offline_path = ::QString::fromStdString(fmt::format(
      fmt::runtime(fmt_offline),
      fmt::arg("x", reply->tileSpec().x()),
      fmt::arg("y", reply->tileSpec().y()),
      fmt::arg("z", reply->tileSpec().zoom())
    ));
    auto const target_path = ::QUrl(offline_path).isLocalFile()
      ? ::QUrl(offline_path).toLocalFile()
      : offline_path;
    auto const path = ::QFileInfo(target_path).absolutePath();
    if(::QFile::exists(target_path) or not ::QDir().mkpath(path))
      return;
    auto f = ::QFile(target_path);
    if(not f.open(::QIODevice::WriteOnly)) {
      llog::warn("geoservice::CGeoTileFetcherMap: failed to open {}", target_path.toStdString());
      return;
    }
    f.write(reply->mapImageData());
    f.close();
  }
} // namespace corona::bootstrap::geoservice
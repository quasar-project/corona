#include <geoservice/class_geotiledreplymap.hh>

#include <qfile.h>

namespace corona::modules::geoservice
{
  using namespace Qt::Literals;

  CGeoTiledMapReply::CGeoTiledMapReply(
    ::QNetworkReply* reply,
    ::QGeoTileSpec const& spec,
    ::QString const& image_format,
    ::QObject* parent
  )
    : ::QGeoTiledMapReply(spec, parent) {
    if(not reply) {
      this->setError(::QGeoTiledMapReply::Error::UnknownError, u"Null reply"_qs);
      return;
    }
    ::QObject::connect(reply, &::QNetworkReply::finished, this, &CGeoTiledMapReply::networkReplyFinished);
    ::QObject::connect(reply, &::QNetworkReply::errorOccurred, this, &CGeoTiledMapReply::networkReplyError);
    ::QObject::connect(this, &::QGeoTiledMapReply::aborted, reply, &::QNetworkReply::abort);
    ::QObject::connect(this, &::QObject::destroyed, reply, &::QObject::deleteLater);
    this->setMapImageFormat(image_format);
  }

  void CGeoTiledMapReply::networkReplyFinished() {
    auto* reply = dynamic_cast<::QNetworkReply*>(this->sender());
    if(not reply or reply->error() != ::QNetworkReply::NoError)
      return;
    this->setMapImageData(reply->readAll());
    this->setFinished(true);
    reply->deleteLater();
  }

  void CGeoTiledMapReply::networkReplyError(::QNetworkReply::NetworkError error) {
    auto* reply = dynamic_cast<::QNetworkReply*>(this->sender());
    reply->deleteLater();
    if(error == ::QNetworkReply::OperationCanceledError)
      this->setFinished(true);
    else
      this->setError(::QGeoTiledMapReply::CommunicationError, reply->errorString());
  }
} // namespace corona::modules::geoservice
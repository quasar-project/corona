//
// Created by user on 07.01.2024.
//

#include "openstreetmapprovider.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

namespace map::provider
{
  auto to_string(const OpenStreetMapConfiguration::ImageFormat format) -> string
  {
    switch(format)
    {
      case OpenStreetMapConfiguration::ImageFormat::Png: return "png";
      case OpenStreetMapConfiguration::ImageFormat::Jpeg: return "jpg";
      default: return "none";
    }
  }

  auto to_string(const OpenStreetMapConfiguration::QtImageFormat format) -> string
  {
    switch(format)
    {
      case OpenStreetMapConfiguration::QtImageFormat::Indexed8: return "Indexed8";
      default: return "none";
    }
  }

  OpenStreetMapConfigurationFile::OpenStreetMapConfigurationFile(const OpenStreetMapConfiguration& configuration, const string_view name)
  {
    this->dictionary = unordered_map<string, string> {
          { "UrlTemplate", configuration.url_template },
          { "ImageFormat", to_string(configuration.image_format) },
          { "QImageFormat", to_string(configuration.qt_image_format) },
          { "MaximumZoomLevel", std::to_string(configuration.max_zoom_level) },
          { "ID", configuration.id },
          { "MapCopyRight", configuration.map_copyright },
          { "DataCopyRight", configuration.data_copyright },
          { "Timestamp", configuration.timestamp }
    };
    this->name = string(name);
  }

  auto OpenStreetMapConfigurationFile::to_json_document() const -> QJsonDocument
  {
    QJsonObject object;
    for(const auto& [key, value] : this->dictionary)
      object.insert(QString::fromStdString(key), QString::fromStdString(value));
    return QJsonDocument(object);
  }

  auto OpenStreetMapConfigurationFile::to_json_string() const -> QString
  {
    return this->to_json_document().toJson(QJsonDocument::Indented);
  }

  void OpenStreetMapConfigurationFile::write_to_file(string_view directory) const
  {
    QFile file(QString::fromStdString(std::format("{}/{}", directory, this->name)));
    file.open(QIODevice::WriteOnly);
    file.write(this->to_json_string().toUtf8());
  }

  OpenStreetMapProvider::OpenStreetMapProvider(const OpenStreetMapConfigurationFiles& cfg, const string_view directory,
    const DirectoryFormat format)
  {
    this->m_directory = format == DirectoryFormat::Relative ?
          QString::fromStdString(std::format("{}/{}", QCoreApplication::applicationDirPath().toStdString(), directory))
        : QString::fromStdString(string(directory));

    const QDir dir(m_directory);
    if(not dir.exists())
      (void)dir.mkpath(dir.path());

    if(dir.isEmpty())
      for(const auto& file : cfg)
        file.write_to_file(this->m_directory.toStdString());
  }

  auto OpenStreetMapProvider::directory() const -> QString { return m_directory; }
}

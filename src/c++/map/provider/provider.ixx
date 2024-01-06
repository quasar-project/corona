module;

#include <afx>
#include <format>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <QtCore/QCoreApplication>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QFile>

export module map.provider;

using namespace std;
using namespace qt;

namespace map::provider
{
  export struct OpenStreetMapConfiguration
  {
    enum class ImageFormat
    {
      Png,
      Jpeg
    };

    enum class QtImageFormat
    {
      Indexed8
    };

    string url_template;
    ImageFormat image_format;
    QtImageFormat qt_image_format;
    string id;
    u8 max_zoom_level;
    string map_copyright;
    string data_copyright;
    string timestamp;
  };

  inline auto to_string(const OpenStreetMapConfiguration::ImageFormat format) -> string
  {
    switch(format)
    {
      case OpenStreetMapConfiguration::ImageFormat::Png: return "png";
      case OpenStreetMapConfiguration::ImageFormat::Jpeg: return "jpg";
      default: return "none";
    }
  }

  inline auto to_string(const OpenStreetMapConfiguration::QtImageFormat format) -> string
  {
    switch(format)
    {
      case OpenStreetMapConfiguration::QtImageFormat::Indexed8: return "Indexed8";
      default: return "none";
    }
  }

  export struct OpenStreetMapConfigurationFile
  {
    OpenStreetMapConfigurationFile(const OpenStreetMapConfiguration& configuration, string_view name)
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

    [[nodiscard]] auto to_json_document() const -> QJsonDocument
    {
      JsonObject object;
      for(const auto& [key, value] : this->dictionary)
        object.insert(String::fromStdString(key), String::fromStdString(value));
      return JsonDocument(object);
    }

    [[nodiscard]] auto to_json_string() const -> String
    {
      return this->to_json_document().toJson(JsonDocument::Indented);
    }

    void write_to_file(const string_view directory) const
    {
      File file(String::fromStdString(std::format("{}/{}", directory, this->name)));
      file.open(IODevice::WriteOnly);
      file.write(this->to_json_string().toUtf8());
    }

    unordered_map<string, string> dictionary;
    string name;
  };

  export using OpenStreetMapConfigurationFiles = vector<OpenStreetMapConfigurationFile>;

  export class OpenStreetMapProvider
  {
    public:
      enum class DirectoryFormat
      {
        Relative,
        Absolute
      };

      explicit OpenStreetMapProvider(
        const OpenStreetMapConfigurationFiles& cfg,
        const string_view directory,
        const DirectoryFormat format
      )
      {
        this->m_directory = format == DirectoryFormat::Relative ?
          String::fromStdString(std::format("{}/{}", CoreApplication::applicationDirPath().toStdString(), directory))
        : String::fromStdString(string(directory));

        for(const auto& file : cfg)
          file.write_to_file(this->m_directory.toStdString().data());
      }

      [[nodiscard]] auto directory() const -> String { return m_directory; }

    private:
      String m_directory;
  };
}
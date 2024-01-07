#pragma once

#include <afx>
#include <string>
#include <unordered_map>
#include <vector>

namespace map::provider
{
  using std::string;
  using std::string_view;
  using std::unordered_map;
  using std::vector;

  struct OpenStreetMapConfiguration
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

  [[nodiscard]] auto to_string(const OpenStreetMapConfiguration::ImageFormat format) -> string;
  [[nodiscard]] auto to_string(const OpenStreetMapConfiguration::QtImageFormat format) -> string;

  struct OpenStreetMapConfigurationFile
  {
    OpenStreetMapConfigurationFile(const OpenStreetMapConfiguration&, string_view name);

    [[nodiscard]] auto to_json_document() const -> qt::JsonDocument;
    [[nodiscard]] auto to_json_string() const -> qt::String;

    void write_to_file(string_view directory) const;

    unordered_map<string, string> dictionary;
    string name;
  };

  using OpenStreetMapConfigurationFiles = vector<OpenStreetMapConfigurationFile>;

  class OpenStreetMapProvider
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
      );

      [[nodiscard]] auto directory() const -> qt::String;

    private:
      qt::String m_directory;
  };
}


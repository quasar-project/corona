//
// Created by user on 07.01.2024.
//

#include "providerqml.h"
#include <array>
#include <string>
#include <ranges>
#include <QtCore/QDir>

using namespace std;

namespace map::provider
{
  OpenStreetMapProviderQML::OpenStreetMapProviderQML(qt::Object* parent)
    : qt::Object(parent)
  {
    constexpr array<string_view, 7> names = { "cycle", "hiking", "night-transit", "satellite", "street", "terrain", "transit" };
    constexpr array<string_view, 7> urls = {
      "https://mt.google.com/vt/lyrs=r&hl=ru&z=%z&x=%x&y=%y",
      "https://mt.google.com/vt/lyrs=r&hl=ru&z=%z&x=%x&y=%y",
      "https://mt.google.com/vt/lyrs=y&hl=ru&z=%z&x=%x&y=%y",
      "https://mt.google.com/vt/lyrs=s&hl=ru&z=%z&x=%x&y=%y",
      "file:///C:/Users/0/Documents/!release/tiles/%z/%x/%y.png", //todo: from config
      "https://mt.google.com/vt/lyrs=p&hl=ru&z=%z&x=%x&y=%y",
      "https://mt.google.com/vt/lyrs=y&hl=ru&z=%z&x=%x&y=%y"
    };
    assert(names.size() == urls.size());

    auto path = "config/osm";
    auto cfg = OpenStreetMapConfiguration {
      .image_format = OpenStreetMapConfiguration::ImageFormat::Png,
      .qt_image_format = OpenStreetMapConfiguration::QtImageFormat::Indexed8,
      .max_zoom_level = 20,
      .map_copyright = "OpenStreetMap contributors",
      .data_copyright = "OpenStreetMap contributors",
      .timestamp = "2022"
    };

    OpenStreetMapConfigurationFiles files;
    for(const auto [i, name] : ranges::views::enumerate(names))
    {
      auto configuration = cfg;
      configuration.url_template =urls[i];
      files.emplace_back(configuration, name);
    }

    this->m_provider.reset(new OpenStreetMapProvider(
      files,
      path,
      OpenStreetMapProvider::DirectoryFormat::Relative
    ));
  }

  OpenStreetMapProviderQML::~OpenStreetMapProviderQML() = default;

  QString OpenStreetMapProviderQML::directory() const { return this->m_provider.get()->directory(); }
}

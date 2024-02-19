//
// Created by user on 07.01.2024.
//

#include "providerqml.h"
#include <array>
#include <ranges>
#include <string>
#include <config/config.h>

using namespace std;

namespace map::provider
{
  OpenStreetMapProviderQML::OpenStreetMapProviderQML(QObject* parent)
    : QObject(parent)
  {
    constexpr array<string_view, 7> names = { "cycle", "hiking", "night-transit", "satellite", "street", "terrain", "transit" };
    constexpr array<string_view, 7> urls = {
      "https://mt.google.com/vt/lyrs=r&hl=ru&z=%z&x=%x&y=%y",
      "https://mt.google.com/vt/lyrs=r&hl=ru&z=%z&x=%x&y=%y",
      "https://mt.google.com/vt/lyrs=y&hl=ru&z=%z&x=%x&y=%y",
      "https://mt.google.com/vt/lyrs=s&hl=ru&z=%z&x=%x&y=%y",
      "https://mt.google.com/vt/lyrs=r&hl=ru&z=%z&x=%x&y=%y",
      //"file:///C:/Users/0/Documents/!release/tiles/%z/%x/%y.png", //todo: from config
      "https://mt.google.com/vt/lyrs=p&hl=ru&z=%z&x=%x&y=%y",
      "https://mt.google.com/vt/lyrs=y&hl=ru&z=%z&x=%x&y=%y"
    };
    assert(names.size() == urls.size());

    auto path = fmt::format("{}/osm", config::CONFIG_DIRECTORY_NAME);
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

  int OpenStreetMapProviderQML::getMapMode(const MapMode mode)
  {
    switch(mode)
    {
      case MapMode::Offline: return 5;
      case MapMode::Scheme: return 0;
      case MapMode::Satellite: return 1;
      case MapMode::Hybrid: return 4;
      default: return -1;
    }
  }
}

//
// Created by user on 06.01.2024.
//

#include "corona.h"
#include <QtGui/QFontDatabase>
#include <QtQml/QmlTypeAndRevisionsRegistration>
#include <map/provider/providerqml.h>

namespace application
{
  auto Corona::instance() -> Corona* { return dynamic_cast<Corona*>(qt::Application::instance()); }
  auto Corona::ref() -> Corona& { return *instance(); }

  Corona::Corona(int argc, char** argv)
    : qt::Application(argc, argv)
  {}

  void Corona::start()
  {
    const auto font_id = qt::FontDatabase::addApplicationFont(":/fonts/Overpass.ttf");
    auto font_list = qt::FontDatabase::applicationFontFamilies(font_id);
    const auto family = font_list.first();
    qt::GuiApplication::setFont(qt::Font(family));
  }

  void Corona::register_types()
  {
    qmlRegisterModule("Corona.Map.Provider", 1, 0);
    qmlRegisterType<map::provider::OpenStreetMapProviderQML>("Corona.Map.Provider", 1, 0, "OpenStreetMapProvider");
  }
}

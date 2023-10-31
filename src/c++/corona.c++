//
// Created by whs31 on 27.10.2023.
//

#include "corona.h"
#include <QtExtensions/QtExtensions>
#include <QtExtensionsToolkit/Register>
#include <QtExtensionsToolkit/FontProvider>
#include <QtExtensionsToolkit/Theme>
#include <Quasar/Quasar>

Corona::Corona(Qt::Object* parent)
  : Qtx::ApplicationBase(parent)
{}

void Corona::start()
{
  Qtx::registerTypes();
  Qtx::Toolkit::registerTypes();
  //Qtx::FontProvider::setApplicationFont(":/qtx/fonts/Overpass.ttf");
  auto theme = Qtx::Theme::get();
  //theme->setFallback(":/theme/gruvbox.json");
  //theme->setName("Gruvbox");
  Quasar::registerTypes();
}

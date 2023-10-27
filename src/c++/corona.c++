//
// Created by whs31 on 27.10.2023.
//

#include "corona.h"
#include <QtExtensions/QtExtensions>
#include <QtExtensionsToolkit/Register>

Corona::Corona(Qt::Object* parent)
  : Qtx::ApplicationBase(parent)
{}

void Corona::start()
{
  Qtx::registerTypes();
  Qtx::Toolkit::registerTypes();
}

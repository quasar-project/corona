//
// Created by user on 06.01.2024.
//

#include "corona.h"

namespace application
{
  auto Corona::instance() -> Corona* { return dynamic_cast<Corona*>(qt::Application::instance()); }
  auto Corona::ref() -> Corona& { return *instance(); }

  Corona::Corona(int argc, char** argv)
    : qt::Application(argc, argv)
  {
  }

  void Corona::start()
  {
  }

  void Corona::register_types()
  {
  }
}

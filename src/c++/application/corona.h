//
// Created by user on 06.01.2024.
//

// ReSharper disable CppClassCanBeFinal
#pragma once

#include <afx>
#include <QtWidgets/QApplication>
#include "base.h"

namespace application
{
  class Corona : public qt::Application,
                 public Base
  {
    Q_OBJECT

    public:
      [[nodiscard]] static auto instance() -> Corona*;
      [[nodiscard]] static auto ref() -> Corona&;

      explicit Corona(int argc, char** argv);

      virtual void start() override;
      virtual void register_types() override;
  };
}

//
// Created by user on 06.01.2024.
//

// ReSharper disable CppClassCanBeFinal
#pragma once

#include <afx>
#include <memory>
#include <QtWidgets/QApplication>
#include "base.h"

namespace config
{
  class Config;
  class ConfigQMLWrapper;
}

namespace application
{
  using std::unique_ptr;

  class Corona : public qt::Application,
                 public Base
  {
    Q_OBJECT

    public:
      [[nodiscard]] static auto instance() -> Corona*;
      [[nodiscard]] static auto ref() -> Corona&;

      explicit Corona(int argc, char** argv);
      virtual ~Corona() override;

      virtual void start() override;
      virtual void register_types() override;

      [[nodiscard]] auto config() const -> config::Config&;

    private:
      unique_ptr<::config::Config> m_config;
      unique_ptr<::config::ConfigQMLWrapper> m_config_wrapper;
  };
}

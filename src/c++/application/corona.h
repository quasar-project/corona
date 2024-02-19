//
// Created by user on 06.01.2024.
//

// ReSharper disable CppClassCanBeFinal
#pragma once

#include <memory>
#include <QtWidgets/QApplication>
#include "base.h"

namespace config
{
  class Config;
  class ConfigQMLWrapper;
}

namespace gui
{
  namespace theme
  {
    class ThemeProvider;
  }
}

namespace application
{
  using std::unique_ptr;

  class Corona : public QApplication,
                 public Base
  {
    Q_OBJECT

    public:
      [[nodiscard]] static auto instance() -> Corona*;
      [[nodiscard]] static auto ref() -> Corona&;

      explicit Corona(int argc, char** argv);
      virtual ~Corona() override;

      virtual auto start() -> void override;
      virtual auto register_types() -> void override;

      [[nodiscard]] auto config() const -> config::Config&;
      [[nodiscard]] auto config_wrapper() const -> config::ConfigQMLWrapper&;

    private:
      unique_ptr<::config::Config> m_config;
      unique_ptr<::config::ConfigQMLWrapper> m_config_wrapper;
      unique_ptr<::gui::theme::ThemeProvider> m_theme_provider;
  };
}

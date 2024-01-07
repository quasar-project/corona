//
// Created by user on 07.01.2024.
//

#include "configqmlwrapper.h"
#include <QtCore/QFile>
#include <QtCore/QVariant>

namespace config
{
  ConfigQMLWrapper::ConfigQMLWrapper(qt::Object* parent)
    : qt::Object(parent),
      m_ptr(nullptr)
  {}

  QVariant ConfigQMLWrapper::value(const QString& category, const QString& name) const
  {
    if(this->m_ptr == nullptr)
    {
      spdlog::error("nullptr in config wrapper");
      return {};
    }
    // todo: replace with checked

    const auto str = this->m_ptr->get_unchecked<string>(category.toStdString(), name.toStdString());
    return qt::Variant(qt::String::fromStdString(str));
  }

  void ConfigQMLWrapper::set_source_ptr(Config* ptr)
  {
    if(ptr != nullptr)
    {
      spdlog::trace("config qml wrapper received ptr");
      this->m_ptr = ptr;
    }
    else
      spdlog::error("invalid ptr received");
  }

  void ConfigQMLWrapper::value_changed_callback(const string& category, const string& name)
  {
    emit valueChanged(qt::String::fromStdString(category), qt::String::fromStdString(name));
  }

  bool ConfigQMLWrapper::create_default_config_callback(const string_view path)
  {
    return qt::File::copy(":/configuration/config.yaml", path.data());
  }
}

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
    ensure_or(this->m_ptr != nullptr, "nullptr in config wrapper", {});

    const auto str = this->m_ptr->get<string>(category.toStdString(), name.toStdString());;
    if(not str.has_value())
    {
      llerror("getting {}/{} failed, reason: {}", category, name, str.error());
      return {};
    }
    return qt::Variant(qt::String::fromStdString(str.value()));
  }

  template<typename T>
  void set_ptr_conv(Config* ptr, const qt::String& category, const qt::String& name, const qt::Variant& value)
  {
    ptr->set(category.toStdString(), name.toStdString(), value.value<T>());
  }

  template<typename T>
  void set_ptr_conv_v(Config* ptr, const qt::String& category, const qt::String& name, const T& value)
  {
    ptr->set(category.toStdString(), name.toStdString(), value);
  }

  void ConfigQMLWrapper::setValue(const QString& category, const QString& name, const QVariant& value)
  {
    ensure(this->m_ptr != nullptr, "nullptr in config wrapper");
    switch(auto type = value.metaType().underlyingType().id())
    {
      case QMetaType::QString: set_ptr_conv_v<string>(this->m_ptr, category, name, value.toString().toStdString()); break;
      case QMetaType::Bool: set_ptr_conv<bool>(this->m_ptr, category, name, value); break;
      case QMetaType::Double: set_ptr_conv<f64>(this->m_ptr, category, name, value); break;
      case QMetaType::Float: set_ptr_conv<f32>(this->m_ptr, category, name, value); break;
      case QMetaType::QByteArray: set_ptr_conv_v<string>(this->m_ptr, category, name, value.toByteArray().toStdString()); break;
      case QMetaType::ULongLong: set_ptr_conv<u64>(this->m_ptr, category, name, value); break;
      case QMetaType::UInt: [[fallthrough]];
      case QMetaType::ULong: [[fallthrough]];
      case QMetaType::UShort: set_ptr_conv<u32>(this->m_ptr, category, name, value); break;
      case QMetaType::Short: [[fallthrough]];
      case QMetaType::Long: [[fallthrough]];
      case QMetaType::LongLong: [[fallthrough]];
      case QMetaType::Int: set_ptr_conv<int>(this->m_ptr, category, name, value); break;
      case QMetaType::Char: set_ptr_conv<i8>(this->m_ptr, category, name, value); break;
      case QMetaType::UChar: set_ptr_conv<u8>(this->m_ptr, category, name, value); break;
      default: ensure(false, "unsupported type");
    }
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

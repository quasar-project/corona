//
// Created by user on 07.01.2024.
//

#include "configqmlwrapper.h"
#include <QtCore/QFile>
#include <QtCore/QVariant>

template<>
struct fmt::formatter<QString> : fmt::formatter<std::string>
{
  auto format(const QString& str, format_context& ctx) -> decltype(ctx.out())
  {
    return fmt::format_to(ctx.out(), "{}", str.toStdString());
  }
};

namespace config
{
  ConfigQMLWrapper::ConfigQMLWrapper(QObject* parent)
    : QObject(parent),
      m_ptr(nullptr)
  {}

  auto ConfigQMLWrapper::value(const QString& category, const QString& name) const -> QVariant
  {
    if(this->m_ptr == nullptr) {
      llog::error("nullptr in config wrapper");
      return {};
    }

    const auto str = this->m_ptr->get<string>(category.toStdString(), name.toStdString());;
    if(not str.has_value())
    {
      llog::error("getting {}/{} failed, reason: {}", category, name, str.error());
      return {};
    }
    return QVariant(QString::fromStdString(str.value()));
  }

  template<typename T>
  auto set_ptr_conv(Config* ptr, const QString& category, const QString& name, const QVariant& value) -> void
  {
    ptr->set(category.toStdString(), name.toStdString(), value.value<T>());
  }

  template<typename T>
  auto set_ptr_conv_v(Config* ptr, const QString& category, const QString& name, const T& value) -> void
  {
    ptr->set(category.toStdString(), name.toStdString(), value);
  }

  auto ConfigQMLWrapper::setValue(const QString& category, const QString& name, const QVariant& value) const -> void
  {
    if(this->m_ptr == nullptr) {
      llog::error("nullptr in config wrapper");
      return;
    }
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
      default: llog::error("unknown type: {}", type);
    }
  }

  auto ConfigQMLWrapper::save() const -> void { this->m_ptr->save(); }
  auto ConfigQMLWrapper::load() const -> void { this->m_ptr->load(); }

  auto ConfigQMLWrapper::set_source_ptr(Config* ptr) -> void
  {
    if(ptr != nullptr)
    {
      llog::trace("config qml wrapper received ptr");
      this->m_ptr = ptr;
    }
    else
      llog::critical("invalid ptr received");
  }

  void ConfigQMLWrapper::value_changed_callback(const string& category, const string& name)
  {
    emit valueChanged(QString::fromStdString(category), QString::fromStdString(name));
  }

  auto ConfigQMLWrapper::create_default_config_callback(const string_view path) -> bool {
    return QFile::copy(":/configuration/config.yaml", path.data());
  }
}

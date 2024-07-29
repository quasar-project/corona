#include <corona-standalone/app/class_dirs_wrapper.hh>

#include <qcoreapplication.h>

namespace corona::standalone::qml
{
  CApplicationDirsWrapper::CApplicationDirsWrapper(
    std::string_view const qualifier,
    std::string_view const organization,
    std::string_view const application,
    ::QObject* parent
  ) : QObject(parent)
    , dirs_(qualifier, organization, application)
  {}

  CApplicationDirsWrapper::CApplicationDirsWrapper(
    fl::meta::project_meta const& meta,
    ::QObject* parent
  ) : QObject(parent)
    , dirs_(meta)
  {}

  CApplicationDirsWrapper::~CApplicationDirsWrapper() = default;

  auto CApplicationDirsWrapper::cache() const -> ::QString { return ::QString::fromStdString(this->dirs_.cache_dir().generic_string()); }
  auto CApplicationDirsWrapper::config() const -> ::QString { return ::QString::fromStdString(this->dirs_.config_dir().generic_string()); }
  auto CApplicationDirsWrapper::configLocal() const -> ::QString { return ::QString::fromStdString(this->dirs_.config_local_dir().generic_string()); }
  auto CApplicationDirsWrapper::data() const -> ::QString { return ::QString::fromStdString(this->dirs_.data_dir().generic_string()); }
  auto CApplicationDirsWrapper::dataLocal() const -> ::QString { return ::QString::fromStdString(this->dirs_.data_local_dir().generic_string()); }
  auto CApplicationDirsWrapper::preferences() const -> ::QString { return ::QString::fromStdString(this->dirs_.preference_dir().generic_string()); }
  auto CApplicationDirsWrapper::runtime() const -> ::QString {
    return this->dirs_.runtime_dir()
      ? ::QString::fromStdString(this->dirs_.runtime_dir()->generic_string())
      : ::QString();
  }
  auto CApplicationDirsWrapper::state() const -> ::QString {
    return this->dirs_.state_dir()
      ? ::QString::fromStdString(this->dirs_.state_dir()->generic_string())
      : ::QString();
  }
  auto CApplicationDirsWrapper::applicationDir() const -> ::QString { return ::QCoreApplication::applicationDirPath(); }
} // namespace corona::standalone::qml
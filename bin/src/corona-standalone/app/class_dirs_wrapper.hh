#pragma once

#include <qobject.h>
#include <floppy/directories.h>

namespace corona::standalone::qml
{
  class CApplicationDirsWrapper : public ::QObject
  {
    Q_OBJECT
    Q_PROPERTY(QString cache READ cache CONSTANT FINAL STORED false)
    Q_PROPERTY(QString config READ config CONSTANT FINAL STORED false)
    Q_PROPERTY(QString configLocal READ configLocal CONSTANT FINAL STORED false)
    Q_PROPERTY(QString data READ data CONSTANT FINAL STORED false)
    Q_PROPERTY(QString dataLocal READ dataLocal CONSTANT FINAL STORED false)
    Q_PROPERTY(QString preferences READ preferences CONSTANT FINAL STORED false)
    Q_PROPERTY(QString runtime READ runtime CONSTANT FINAL STORED false)
    Q_PROPERTY(QString state READ state CONSTANT FINAL STORED false)
    Q_PROPERTY(QString applicationDir READ applicationDir CONSTANT FINAL STORED false)

   public:
    explicit CApplicationDirsWrapper(fl::filesystem::application_dirs* dirs, ::QObject* parent = nullptr);
    virtual ~CApplicationDirsWrapper() override;

    [[nodiscard]] auto cache() const -> ::QString;
    [[nodiscard]] auto config() const -> ::QString;
    [[nodiscard]] auto configLocal() const -> ::QString;
    [[nodiscard]] auto data() const -> ::QString;
    [[nodiscard]] auto dataLocal() const -> ::QString;
    [[nodiscard]] auto preferences() const -> ::QString;
    [[nodiscard]] auto runtime() const -> ::QString;
    [[nodiscard]] auto state() const -> ::QString;
    [[nodiscard]] auto applicationDir() const -> ::QString;

   private:
    fl::filesystem::application_dirs* dirs_;
  };
} // namespace corona::standalone::qml
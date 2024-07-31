#pragma once

#include <qobject.h>
#include <floppy/directories.h>
#include <corona-standalone/definitions.hh>

namespace corona::standalone::map
{
  class CMapViewManager : public ::QObject // NOLINT(*-special-member-functions)
  {
    Q_OBJECT
    Q_PROPERTY(QString configPath READ configPath WRITE setConfigPath NOTIFY configPathChanged FINAL)
    Q_PROPERTY(QString storagePath READ storagePath WRITE setStoragePath NOTIFY storagePathChanged FINAL)

   public:
    enum class MapMode : char
    {
      Street,
      Satellite,
      Hybrid,
      Auto = 127
    };
    Q_ENUM(MapMode)

    explicit CMapViewManager(fl::filesystem::application_dirs const& dirs, ::QObject* parent = nullptr);
    virtual ~CMapViewManager() override = default;

    [[nodiscard]] auto configPath() const -> ::QString { return this->config_path_; } // NOLINT(*-identifier-naming)
    auto setConfigPath(::QString const& path) -> void { // NOLINT(*-identifier-naming)
      this->config_path_ = path;
      emit this->configPathChanged();
    }

    [[nodiscard]] auto storagePath() const -> ::QString { return this->storage_path_; } // NOLINT(*-identifier-naming)
    auto setStoragePath(::QString const& path) -> void { // NOLINT(*-identifier-naming)
      this->storage_path_ = path;
      emit this->storagePathChanged();
    }

   signals:
    void configPathChanged(); // NOLINT(*-identifier-naming)
    void storagePathChanged(); // NOLINT(*-identifier-naming)

   private:
    ::QString config_path_;
    ::QString storage_path_;
  };
} // namespace corona::standalone::map

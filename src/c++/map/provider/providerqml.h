//
// Created by user on 07.01.2024.
//

#pragma once

#include <afx>
#include <memory>
#include <QtCore/QObject>

#if defined CXX20_MODULES_DISABLED
#include "openstreetmapprovider.h"
#else
#ifndef Q_MOC_RUN
import map.provider;
#endif
#endif

namespace map::provider
{
  using std::unique_ptr;

  // ReSharper disable once CppClassCanBeFinal
  class OpenStreetMapProviderQML : public qt::Object
  {
    Q_OBJECT
    Q_PROPERTY(QString directory READ directory CONSTANT FINAL)

    public:
      enum class MapMode
      {
        Offline,
        Hybrid,
        Satellite,
        Scheme
      };
      Q_ENUM(MapMode)

      explicit OpenStreetMapProviderQML(qt::Object* parent = nullptr);
      virtual ~OpenStreetMapProviderQML() override;

      [[nodiscard]] QString directory() const;

    public:
      invokable static int getMapMode(MapMode);

    private:
      unique_ptr<OpenStreetMapProvider> m_provider;
  };
}
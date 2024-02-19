//
// Created by user on 07.01.2024.
//

#pragma once

#include <memory>
#include <QtCore/QObject>
#include "openstreetmapprovider.h"

namespace map::provider
{
  using std::unique_ptr;

  // ReSharper disable once CppClassCanBeFinal
  class OpenStreetMapProviderQML : public QObject
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

      explicit OpenStreetMapProviderQML(QObject* parent = nullptr);
      virtual ~OpenStreetMapProviderQML() override;

      [[nodiscard]] QString directory() const;

    public:
      Q_INVOKABLE static int getMapMode(MapMode);

    private:
      unique_ptr<OpenStreetMapProvider> m_provider;
  };
}
//
// Created by user on 07.01.2024.
//

#pragma once

#include <afx>
#include <memory>
#include <QtCore/QObject>

#ifndef Q_MOC_RUN
import map.provider;
#endif

namespace map::provider
{
  // ReSharper disable once CppClassCanBeFinal
  class OpenStreetMapProviderQML : public qt::Object
  {
    Q_OBJECT
    Q_PROPERTY(QString directory READ directory CONSTANT FINAL)

    public:
      explicit OpenStreetMapProviderQML(qt::Object* parent = nullptr);
      virtual ~OpenStreetMapProviderQML() override;

      [[nodiscard]] QString directory() const;

    private:
      std::unique_ptr<OpenStreetMapProvider> m_provider;
  };
}
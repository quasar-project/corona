//
// Created by user on 07.01.2024.
//

#pragma once

#include <afx>
#include <QtCore/QObject>

import map.provider;

namespace map::provider
{
  // ReSharper disable once CppClassCanBeFinal
  class OpenStreetMapProviderQML : public qt::Object
  {
    Q_OBJECT
    Q_PROPERTY(QString directory READ directory CONSTANT FINAL)

    public:
      explicit OpenStreetMapProviderQML(qt::Object* parent = nullptr);

      [[nodiscard]] QString directory() const;

    private:
      OpenStreetMapProvider m_provider;
  };
}
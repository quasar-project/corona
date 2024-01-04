//
// Created by user on 05.01.2024.
//

//
// Created by whs31 on 27.10.2023.
//

#pragma once

#include <qol/qol>
#include <QtCore/QObject>

namespace launcher
{
  class ApplicationBase : public Qt::Object
  {
    Q_OBJECT

    public:
      explicit ApplicationBase(Qt::Object* parent = nullptr);
      virtual void register_types() = 0;
      virtual void start() = 0;
  };
}

namespace launcher
{
  inline ApplicationBase::ApplicationBase(Qt::Object* parent)
    : Qt::Object(parent)
  {}
} // QtEx

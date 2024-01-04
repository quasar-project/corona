//
// Created by whs31 on 27.10.2023.
//

#pragma once

#include <launcher/application_base.h>

// ReSharper disable once CppClassCanBeFinal
class Corona : public launcher::ApplicationBase
{
  Q_OBJECT

  public:
    explicit Corona(Qt::Object* parent = nullptr);

    virtual void start() override;
    virtual void register_types() override;
};



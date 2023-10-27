//
// Created by whs31 on 27.10.2023.
//

#pragma once

#include <QtExtensionsToolkit/ApplicationBase>

class Corona : public Qtx::ApplicationBase
{
  Q_OBJECT

  public:
    explicit Corona(Qt::Object* parent = nullptr);

    void start() override;
};



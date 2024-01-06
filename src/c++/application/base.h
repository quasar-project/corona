//
// Created by user on 06.01.2024.
//

#pragma once

#include <afx>

namespace application
{
  class Base
  {
    public:
      virtual ~Base() = default;
      virtual void register_types() = 0;
      virtual void start() = 0;
  };
}
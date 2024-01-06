//
// Created by user on 06.01.2024.
//

#pragma once

#include <spdlog/spdlog.h>
#include <QtCore/QString>

namespace logging = spdlog;

template<>
struct fmt::formatter<QString> : fmt::formatter<std::string>
{
  auto format(const QString& str, format_context& ctx) -> decltype(ctx.out())
  {
    return fmt::format_to(ctx.out(), "{}", str.toStdString());
  }
};


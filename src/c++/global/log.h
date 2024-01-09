//
// Created by user on 06.01.2024.
//

#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>
#include <QtCore/QString>

#define lltrace(...) SPDLOG_TRACE(__VA_ARGS__)
#define lldebug(...) SPDLOG_DEBUG(__VA_ARGS__)
#define llinfo(...) SPDLOG_INFO(__VA_ARGS__)
#define llwarn(...) SPDLOG_WARN(__VA_ARGS__)
#define llcritical(...) SPDLOG_CRITICAL(__VA_ARGS__)
#define llerror(...) SPDLOG_ERROR(__VA_ARGS__)

namespace logging = spdlog;

template<>
struct fmt::formatter<QString> : fmt::formatter<std::string>
{
  auto format(const QString& str, format_context& ctx) -> decltype(ctx.out())
  {
    return fmt::format_to(ctx.out(), "{}", str.toStdString());
  }
};


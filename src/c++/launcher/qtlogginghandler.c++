//
// Created by user on 07.01.2024.
//

#include <leaf/global.h>
#include <QtCore/QDebug>
#include "qtlogginghandler.h"

// todo: move to leaf or smth
template<>
struct fmt::formatter<QString> : fmt::formatter<std::string>
{
  auto format(const QString& str, format_context& ctx) -> decltype(ctx.out())
  {
    return fmt::format_to(ctx.out(), "{}", str.toStdString());
  }
};

namespace launcher
{
  void logging_handler(QtMsgType type, const QMessageLogContext&, const QString& msg)
  {
    switch(type)
    {
      case QtDebugMsg: llog::debug("{}", msg); break;
      case QtInfoMsg: llog::info("{}", msg); break;
      case QtWarningMsg: llog::warn("{}", msg); break;
      case QtCriticalMsg: llog::critical("{}", msg); break;
      case QtFatalMsg: llog::error("{}", msg); break;
      default: break;
    }
  }
}

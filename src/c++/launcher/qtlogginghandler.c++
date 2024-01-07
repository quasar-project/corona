//
// Created by user on 07.01.2024.
//

#include "qtlogginghandler.h"
#include <QtCore/QDebug>

namespace launcher
{
  void logging_handler(QtMsgType type, const QMessageLogContext&, const qt::String& msg)
  {
    switch(type)
    {
      case QtDebugMsg: logging::debug("{}", msg); break;
      case QtInfoMsg: logging::info("{}", msg); break;
      case QtWarningMsg: logging::warn("{}", msg); break;
      case QtCriticalMsg: logging::critical("{}", msg); break;
      case QtFatalMsg: logging::error("{}", msg); break;
      default: break;
    }
  }
}

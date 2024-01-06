//
// Created by user on 06.01.2024.
//

#pragma once

#include <afx>
#include <QtCore/QDebug>

namespace launcher
{
  inline void logging_handler(QtMsgType type, const QMessageLogContext&, const QString& msg)
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

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
      case QtDebugMsg: lldebug("{}", msg); break;
      case QtInfoMsg: llinfo("{}", msg); break;
      case QtWarningMsg: llwarn("{}", msg); break;
      case QtCriticalMsg: llcritical("{}", msg); break;
      case QtFatalMsg: llerror("{}", msg); break;
      default: break;
    }
  }
}

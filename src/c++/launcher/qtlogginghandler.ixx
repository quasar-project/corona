module;

#include <afx>
#include <QtCore/QDebug>

export module launcher.qtlog;

namespace launcher
{
  export inline void logging_handler(QtMsgType type, const QMessageLogContext&, const QString& msg)
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
module;

#include <afx>
#include <QtCore/QDebug>

export module launcher.qt;

namespace launcher
{
  export inline void logging_handler(QtMsgType type, const QMessageLogContext&, const QString& msg)
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
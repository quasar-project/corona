#pragma once

class QMessageLogContext;

#if !defined(Q_CC_MINGW)
enum QtMsgType : int;
#else
enum QtMsgType;
#endif

namespace launcher
{
  void logging_handler(QtMsgType, const QMessageLogContext&, const QString&);
}

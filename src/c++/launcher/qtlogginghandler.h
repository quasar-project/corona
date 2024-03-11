#pragma once

class QMessageLogContext;

#if !defined(Q_CC_MINGW) && !defined(Q_CC_GNU)
enum QtMsgType : int;
#else
enum QtMsgType;
#endif

namespace launcher
{
  void logging_handler(QtMsgType, const QMessageLogContext&, const QString&);
}

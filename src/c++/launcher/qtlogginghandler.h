#pragma once

#include <afx>

class QMessageLogContext;
enum QtMsgType : int;

namespace launcher
{
  void logging_handler(QtMsgType, const QMessageLogContext&, const qt::String&);
}

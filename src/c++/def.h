//
// Created by user on 06.01.2024.
//

#pragma once

#define invokable Q_INVOKABLE
#define connectable_slot Q_SLOT

class QObject;
class QString;

namespace qt
{
  using Object = QObject;
  using String = QString;
}
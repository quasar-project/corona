#pragma once

#include <afx>
#include <QtCore/QObject>
#include "config.h"

namespace config
{
  // ReSharper disable once CppClassCanBeFinal
  class ConfigQMLWrapper : public qt::Object
  {
    Q_OBJECT

    public:
      explicit ConfigQMLWrapper(qt::Object* parent);

      invokable QVariant value(const QString& category, const QString& name) const;

      void set_source_ptr(Config* ptr);
      void value_changed_callback(const string& category, const string& name);

      static bool create_default_config_callback(string_view path);

      signals:
        void valueChanged(qt::String category, qt::String name);

    private:
      Config* m_ptr;
  };
}


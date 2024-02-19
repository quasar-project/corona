// ReSharper disable CppEnforceFunctionDeclarationStyle
#pragma once

#include <QtCore/QObject>
#include "config.h"

namespace config
{
  class ConfigQMLWrapper;

  // ReSharper disable once CppClassCanBeFinal
  class ConfigQMLWrapper : public QObject
  {
    Q_OBJECT

    public:
      explicit ConfigQMLWrapper(QObject* parent);

      Q_INVOKABLE [[nodiscard]] QVariant value(const QString& category, const QString& name) const;
      Q_INVOKABLE void setValue(const QString& category, const QString& name, const QVariant& value) const;
      Q_INVOKABLE void save() const;
      Q_INVOKABLE void load() const;

      void set_source_ptr(Config* ptr);
      void value_changed_callback(const string& category, const string& name);

      static auto create_default_config_callback(string_view path) -> bool;

      signals:
        void valueChanged(QString category, QString name);

    private:
      Config* m_ptr;
  };
}


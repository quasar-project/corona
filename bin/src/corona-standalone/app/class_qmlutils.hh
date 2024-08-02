#pragma once

#include <qobject.h>
#include <qqmlengine.h>
#include <corona-standalone/definitions.hh>

namespace corona::standalone::app
{
  class CQmlUtils : public ::QObject
  {
    Q_OBJECT

   public:
    [[nodiscard]] static auto create(::QQmlEngine* qml_engine, ::QJSEngine* js_engine) -> CQmlUtils*;
    virtual ~CQmlUtils() override;

    Q_INVOKABLE void copyToClipboard(QString const& text);

   private:
    explicit CQmlUtils(::QJSValue id, ::QObject* parent = nullptr);

    ::QJSValue symbol_;
  };
} // namespace corona::standalone::app
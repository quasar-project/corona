#include <corona-standalone/app/class_qmlutils.hh>

#include <qclipboard.h>
#include <qguiapplication.h>
#include <corona/modules/qmlbind/literals.h>
#include <corona-standalone/utility/formatters.hh>

namespace corona::standalone::app
{
  auto CQmlUtils::create(::QQmlEngine* qml_engine, ::QJSEngine* js_engine) -> CQmlUtils* {
    return new CQmlUtils(qml_engine->newSymbol(u"Utils"_qs));
  }

  CQmlUtils::CQmlUtils(::QJSValue id, ::QObject* parent)
    : ::QObject(parent)
    , symbol_(std::move(id))
  {}

  CQmlUtils::~CQmlUtils() = default;

  auto CQmlUtils::copyToClipboard(QString const& text) -> void {
    llog::trace("CQmlUtils: copied to clipboard: {}", text);
    QGuiApplication::clipboard()->setText(text);
  }
} // namespace corona::standalone::app
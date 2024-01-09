#pragma once

#include <afx>
#include <QtCore/QObject>

namespace gui::theme
{
  class ThemeProvider;
  class ThemeQMLWrapper : public qt::Object
  {
    Q_OBJECT

    public:
      explicit ThemeQMLWrapper(ThemeProvider* ptr, qt::Object* parent = nullptr);


    private:
      static void emplace();

    private:
      ThemeProvider* m_ptr;
  };
}
#pragma once

#include <corona-standalone/definitions.hh>
#include <corona-standalone/gui/immediate/drawable.hh>

namespace corona::standalone::gui::immediate
{
  class InformationWidget : public Drawable
  {
   public:
    explicit InformationWidget(string_view title, string_view version);

    virtual ~InformationWidget() = default;
    virtual auto draw_debug_frame() -> void override;

   private:
    string m_title;
    string m_version;
  };
} // namespace corona::standalone::gui::immediate
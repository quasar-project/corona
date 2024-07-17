#include <corona-standalone/gui/immediate/info_widget.hh>

#include <corona-standalone/gui/immediate/extensions.hh>

namespace x = im::ext;

namespace corona::standalone::gui::immediate
{
  InformationWidget::InformationWidget(string_view title, string_view version)
    : m_title(title)
    , m_version(version)
  {}

  auto InformationWidget::draw_debug_frame() -> void
  {
    auto const& io = im::GetIO();
    im::SetNextWindowSize({0, io.DisplaySize.x}, ImGuiCond_FirstUseEver);
    im::SetNextWindowPos({0, 0}, ImGuiCond_FirstUseEver);
    im::Begin("Information titlebar", nullptr, ImGuiWindowFlags_NoResize
      | ImGuiWindowFlags_NoMove
      | ImGuiWindowFlags_NoCollapse
      | ImGuiWindowFlags_NoTitleBar
      | ImGuiWindowFlags_NoSavedSettings
      | ImGuiWindowFlags_NoFocusOnAppearing
      | ImGuiWindowFlags_NoBringToFrontOnFocus
      | ImGuiWindowFlags_NoNavFocus
      | ImGuiWindowFlags_NoBackground
    );
    x::text(x::default_palette[x::palette::color::cyan], "{} version {}", this->m_title, this->m_version);
    x::same_line();
    // spacing
    im::SetCursorPosX(im::GetCursorPosX() + 30);
    x::text("Application average {:.2f} ms/frame ({:.1f} FPS)", 1000.0F / io.Framerate, io.Framerate);
    im::End();
  }
} // namespace corona::standalone::gui::immediate
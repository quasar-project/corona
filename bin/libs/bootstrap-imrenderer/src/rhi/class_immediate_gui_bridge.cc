#include <qdebugenv/rhi/class_immediate_gui_bridge.h>

#include <cassert>
#include <qfile.h>
#include <qguiapplication.h>
#include <qevent.h>
#include <qclipboard.h>
#include <floppy/logging.h>
#include <qdebugenv/vendored/imgui.h>

namespace llog = ::floppy::log;

namespace
{
  [[nodiscard]] auto clipboard_text([[maybe_unused]] void* h) -> char const* {
    static auto contents = ::QByteArray();
    contents = ::QGuiApplication::clipboard()->text().toUtf8();
    return contents.constData();
  }

  auto set_clipboard_text([[maybe_unused]] void* h, char const* text) -> void {
    ::QGuiApplication::clipboard()->setText(QString::fromUtf8(text));
  }

  auto update_keyboard_modifiers(Qt::KeyboardModifiers mod) -> void {
    auto& io = ImGui::GetIO();
    io.AddKeyEvent(ImGuiKey_ModCtrl, mod.testFlag(Qt::ControlModifier));
    io.AddKeyEvent(ImGuiKey_ModShift, mod.testFlag(Qt::ShiftModifier));
    io.AddKeyEvent(ImGuiKey_ModAlt, mod.testFlag(Qt::AltModifier));
    io.AddKeyEvent(ImGuiKey_ModSuper, mod.testFlag(Qt::MetaModifier));
  }

  [[nodiscard]] constexpr auto to_imgui_keycode(Qt::Key const key) -> ImGuiKey
  {
    switch(key) {
      case Qt::Key_Space: return ImGuiKey_Space;
      case Qt::Key_Apostrophe: return ImGuiKey_Apostrophe;
      case Qt::Key_Comma: return ImGuiKey_Comma;
      case Qt::Key_Minus: return ImGuiKey_Minus;
      case Qt::Key_Period: return ImGuiKey_Period;
      case Qt::Key_Slash: return ImGuiKey_Slash;
      case Qt::Key_0: return ImGuiKey_0;
      case Qt::Key_1: return ImGuiKey_1;
      case Qt::Key_2: return ImGuiKey_2;
      case Qt::Key_3: return ImGuiKey_3;
      case Qt::Key_4: return ImGuiKey_4;
      case Qt::Key_5: return ImGuiKey_5;
      case Qt::Key_6: return ImGuiKey_6;
      case Qt::Key_7: return ImGuiKey_7;
      case Qt::Key_8: return ImGuiKey_8;
      case Qt::Key_9: return ImGuiKey_9;
      case Qt::Key_Semicolon: return ImGuiKey_Semicolon;
      case Qt::Key_Equal: return ImGuiKey_Equal;
      case Qt::Key_A: return ImGuiKey_A;
      case Qt::Key_B: return ImGuiKey_B;
      case Qt::Key_C: return ImGuiKey_C;
      case Qt::Key_D: return ImGuiKey_D;
      case Qt::Key_E: return ImGuiKey_E;
      case Qt::Key_F: return ImGuiKey_F;
      case Qt::Key_G: return ImGuiKey_G;
      case Qt::Key_H: return ImGuiKey_H;
      case Qt::Key_I: return ImGuiKey_I;
      case Qt::Key_J: return ImGuiKey_J;
      case Qt::Key_K: return ImGuiKey_K;
      case Qt::Key_L: return ImGuiKey_L;
      case Qt::Key_M: return ImGuiKey_M;
      case Qt::Key_N: return ImGuiKey_N;
      case Qt::Key_O: return ImGuiKey_O;
      case Qt::Key_P: return ImGuiKey_P;
      case Qt::Key_Q: return ImGuiKey_Q;
      case Qt::Key_R: return ImGuiKey_R;
      case Qt::Key_S: return ImGuiKey_S;
      case Qt::Key_T: return ImGuiKey_T;
      case Qt::Key_U: return ImGuiKey_U;
      case Qt::Key_V: return ImGuiKey_V;
      case Qt::Key_W: return ImGuiKey_W;
      case Qt::Key_X: return ImGuiKey_X;
      case Qt::Key_Y: return ImGuiKey_Y;
      case Qt::Key_Z: return ImGuiKey_Z;
      case Qt::Key_BracketLeft: return ImGuiKey_LeftBracket;
      case Qt::Key_Backslash: return ImGuiKey_Backslash;
      case Qt::Key_BracketRight: return ImGuiKey_RightBracket;
      case Qt::Key_QuoteLeft: return ImGuiKey_GraveAccent;
      case Qt::Key_Escape: return ImGuiKey_Escape;
      case Qt::Key_Tab: return ImGuiKey_Tab;
      case Qt::Key_Backspace: return ImGuiKey_Backspace;
      case Qt::Key_Return: [[fallthrough]];
      case Qt::Key_Enter: return ImGuiKey_Enter;
      case Qt::Key_Insert: return ImGuiKey_Insert;
      case Qt::Key_Delete: return ImGuiKey_Delete;
      case Qt::Key_Pause: return ImGuiKey_Pause;
      case Qt::Key_Print: return ImGuiKey_PrintScreen;
      case Qt::Key_Home: return ImGuiKey_Home;
      case Qt::Key_End: return ImGuiKey_End;
      case Qt::Key_Left: return ImGuiKey_LeftArrow;
      case Qt::Key_Up: return ImGuiKey_UpArrow;
      case Qt::Key_Right: return ImGuiKey_RightArrow;
      case Qt::Key_Down: return ImGuiKey_DownArrow;
      case Qt::Key_PageUp: return ImGuiKey_PageUp;
      case Qt::Key_PageDown: return ImGuiKey_PageDown;
      case Qt::Key_Shift: return ImGuiKey_LeftShift;
      case Qt::Key_Control: return ImGuiKey_LeftCtrl;
      case Qt::Key_Meta: return ImGuiKey_LeftSuper;
      case Qt::Key_Alt: return ImGuiKey_LeftAlt;
      case Qt::Key_CapsLock: return ImGuiKey_CapsLock;
      case Qt::Key_NumLock: return ImGuiKey_NumLock;
      case Qt::Key_ScrollLock: return ImGuiKey_ScrollLock;
      case Qt::Key_F1: return ImGuiKey_F1;
      case Qt::Key_F2: return ImGuiKey_F2;
      case Qt::Key_F3: return ImGuiKey_F3;
      case Qt::Key_F4: return ImGuiKey_F4;
      case Qt::Key_F5: return ImGuiKey_F5;
      case Qt::Key_F6: return ImGuiKey_F6;
      case Qt::Key_F7: return ImGuiKey_F7;
      case Qt::Key_F8: return ImGuiKey_F8;
      case Qt::Key_F9: return ImGuiKey_F9;
      case Qt::Key_F10: return ImGuiKey_F10;
      case Qt::Key_F11: return ImGuiKey_F11;
      case Qt::Key_F12: return ImGuiKey_F12;
      default:
        break;
    }
    return ImGuiKey_None;
  }
} // namespace

namespace qdebugenv
{
  CImmediateGuiBridge::CImmediateGuiBridge()
    : ctx_(ImGui::CreateContext())
  {
    this->swap_ctx();
    this->rebuild_font_atlas();
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.GetClipboardTextFn = ::clipboard_text;
    io.SetClipboardTextFn = ::set_clipboard_text;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    ImGui::StyleColorsDark();
  }

  CImmediateGuiBridge::~CImmediateGuiBridge() {
    ImGui::DestroyContext(static_cast<ImGuiContext*>(this->ctx_));
  }

  auto CImmediateGuiBridge::next_frame(
    ::QSizeF const& logical_output_size,
    f32 const dpr,
    ::QPointF const& logical_offset,
    CImmediateGuiBridge::FrameFunction frame_fn
  ) -> void
  {
    this->swap_ctx();
    auto& io= ImGui::GetIO();

    auto const item_pixel_offset = logical_offset * dpr;
    this->f_.output_pixel_size = (logical_output_size * dpr).toSize();
    io.DisplaySize.x = static_cast<f32>(logical_output_size.width());
    io.DisplaySize.y = static_cast<f32>(logical_output_size.height());
    io.DisplayFramebufferScale = ImVec2(dpr, dpr);

    ImGui::NewFrame();
    if(frame_fn)
      frame_fn();
    ImGui::Render();

    auto* draw = ImGui::GetDrawData();
    draw->ScaleClipRects(ImVec2(dpr, dpr));
    this->f_.vbuf.resize(draw->CmdListsCount);
    this->f_.ibuf.resize(draw->CmdListsCount);
    this->f_.total_vbuf_size = 0;
    this->f_.total_ibuf_size = 0;
    for(auto n = 0; n < draw->CmdListsCount; ++n) {
      auto const* cmd_list = draw->CmdLists[n];
      auto const vbuf_size = cmd_list->VtxBuffer.Size * sizeof(ImDrawVert);
      this->f_.vbuf[n].offset = this->f_.total_vbuf_size;
      this->f_.total_vbuf_size += vbuf_size;
      auto const ibu_size = cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx);
      this->f_.ibuf[n].offset = this->f_.total_ibuf_size;
      this->f_.total_ibuf_size += ibu_size;
    }
    this->f_.draw.clear();
    for (int n = 0; n < draw->CmdListsCount; ++n) {
      auto const* cmd_list = draw->CmdLists[n];
      this->f_.vbuf[n].data = ::QByteArray(
        reinterpret_cast<char const*>(cmd_list->VtxBuffer.Data), // NOLINT(*-pro-type-reinterpret-cast)
        static_cast<isize>(cmd_list->VtxBuffer.Size * sizeof(ImDrawVert))
      );
      this->f_.ibuf[n].data = ::QByteArray(
        reinterpret_cast<char const*>(cmd_list->IdxBuffer.Data), // NOLINT(*-pro-type-reinterpret-cast)
        static_cast<isize>(cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx))
      );
      ImDrawIdx const* index_buffer_offset = nullptr;
      for(auto i = 0; i < cmd_list->CmdBuffer.Size; ++i) {
        const ImDrawCmd *cmd = &cmd_list->CmdBuffer[i];
        auto const index_offset = this->f_.ibuf[n].offset + reinterpret_cast<uintptr_t>(index_buffer_offset); // NOLINT(*-pro-type-reinterpret-cast)
        if(not cmd->UserCallback) {
          auto const dc = CRhiRenderer::DrawCommand {
            .command_list_buffer_index = n,
            .texture_id = cmd->TextureId,
            .index_offset = static_cast<u32>(index_offset),
            .element_count = cmd->ElemCount,
            .item_pixel_offset = item_pixel_offset,
            .clip_rect = ::QVector4D(cmd->ClipRect.x, cmd->ClipRect.y, cmd->ClipRect.z, cmd->ClipRect.w)
          };
          this->f_.draw.push_back(dc);
        } else
          cmd->UserCallback(cmd_list, cmd);
        index_buffer_offset += cmd->ElemCount; // NOLINT(*-pro-bounds-pointer-arithmetic)
      }
    }
  }

  auto CImmediateGuiBridge::sync_renderer(CRhiRenderer* renderer) -> void {
    assert(renderer and "CImmediateGuiBridge::sync_renderer: renderer is nullptr");
    if(this->sf_.valid()) {
      renderer->sf = this->sf_;
      this->sf_.reset();
    }
    renderer->f = std::move(this->f_);
  }

  auto CImmediateGuiBridge::process_event(::QEvent* event) -> bool
  {
    this->swap_ctx();
    auto& io = ImGui::GetIO();
    switch(event->type()) {
      case ::QEvent::MouseButtonPress: {
        auto* me = dynamic_cast<::QMouseEvent*>(event);
        ::update_keyboard_modifiers(me->modifiers());
        auto buttons = me->buttons();
        if(buttons.testFlag(Qt::LeftButton) and not this->pressed_mouse_buttons_.testFlag(Qt::LeftButton))
          io.AddMouseButtonEvent(0, true);
        if(buttons.testFlag(Qt::RightButton) and not this->pressed_mouse_buttons_.testFlag(Qt::RightButton))
          io.AddMouseButtonEvent(1, true);
        if (buttons.testFlag(Qt::MiddleButton) and not this->pressed_mouse_buttons_.testFlag(Qt::MiddleButton))
          io.AddMouseButtonEvent(2, true);
        this->pressed_mouse_buttons_ = buttons;
      } return true;
      case QEvent::MouseButtonRelease: {
        auto* me = dynamic_cast<::QMouseEvent*>(event);
        Qt::MouseButtons buttons = me->buttons();
        if(not buttons.testFlag(Qt::LeftButton) and this->pressed_mouse_buttons_.testFlag(Qt::LeftButton))
          io.AddMouseButtonEvent(0, false);
        if(not buttons.testFlag(Qt::RightButton) and this->pressed_mouse_buttons_.testFlag(Qt::RightButton))
          io.AddMouseButtonEvent(1, false);
        if(not buttons.testFlag(Qt::MiddleButton) and this->pressed_mouse_buttons_.testFlag(Qt::MiddleButton))
          io.AddMouseButtonEvent(2, false);
        this->pressed_mouse_buttons_ = buttons;
      } return true;
      case QEvent::MouseMove: {
        auto* me = dynamic_cast<::QMouseEvent*>(event);
        auto const pos = me->position();
        io.AddMousePosEvent(static_cast<f32>(pos.x()), static_cast<f32>(pos.y()));
      } return true;
      case QEvent::Wheel: {
        auto* we = dynamic_cast<::QWheelEvent*>(event);
        auto const wheel = ::QPointF(
          static_cast<f32>(we->angleDelta().x()) / 120.0F,
          static_cast<f32>(we->angleDelta().y()) / 120.0F
        );
        io.AddMouseWheelEvent(static_cast<f32>(wheel.x()), static_cast<f32>(wheel.y()));
      } return true;
      case QEvent::KeyPress: [[fallthrough]];
      case QEvent::KeyRelease: {
        auto* ke = dynamic_cast<::QKeyEvent*>(event);
        auto const down = event->type() == ::QEvent::KeyPress;
        ::update_keyboard_modifiers(ke->modifiers());
        io.AddKeyEvent(::to_imgui_keycode(static_cast<Qt::Key>(ke->key())), down);
        if(down and not ke->text().isEmpty()) {
          auto const text = ke->text().toUtf8();
          io.AddInputCharactersUTF8(text.constData());
        }
      } return true;
      default: break;
    }
    return false;
  }

  auto CImmediateGuiBridge::rebuild_font_atlas() -> void {
    this->swap_ctx();
    auto& io = ImGui::GetIO();
    unsigned char* pixels = nullptr;
    auto w = int();
    auto h = int();
    io.Fonts->GetTexDataAsRGBA32(&pixels, &w, &h);
    auto const wrapper_image = ::QImage(const_cast<unsigned char const*>(pixels), w, h, ::QImage::Format_RGBA8888);
    this->sf_.font_texture_data = wrapper_image.copy();
    io.Fonts->SetTexID(nullptr);
  }

  auto CImmediateGuiBridge::rebuild_font_atlas_with_font(std::string_view const filename) {
    auto file = ::QFile(filename.data());
    if(not file.open(::QIODevice::ReadOnly)) {
      llog::error("failed to load font: {}", filename);
      return;
    }
    auto font_data = file.readAll();
    this->swap_ctx();
    auto& io = ImGui::GetIO();
    auto font_config = ::ImFontConfig();
    font_config.FontDataOwnedByAtlas = false;
    io.Fonts->Clear();
    io.Fonts->AddFontFromMemoryTTF(
      font_data.data(),
      static_cast<i32>(font_data.size()),
      20.0F,
      &font_config
    );
    this->rebuild_font_atlas();
  }

  auto CImmediateGuiBridge::swap_ctx() -> void {
    ImGui::SetCurrentContext(static_cast<ImGuiContext*>(this->ctx_));
  }
} // namespace qdebugenv
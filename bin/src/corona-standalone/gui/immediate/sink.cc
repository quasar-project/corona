#include <corona-standalone/gui/immediate/sink.hh>

#include <corona-standalone/gui/immediate/detail/utility.hh>

enum ANSIColors
{
  Reset = 0,
  Black = 30,
  Red = 31,
  Green = 32,
  Yellow = 33,
  Blue = 34,
  Magenta = 35,
  Cyan = 36,
  White = 37,
  BrightBlack = 90,
  BrightRed = 91,
  BrightGreen = 92,
  BrightYellow = 93,
  BrightBlue = 94,
  BrightMagenta = 95,
  BrightCyan = 96,
  BrightWhite = 97
};

namespace corona::standalone::gui::immediate
{
  auto LogSink::create() noexcept -> shared_ptr<LogSink> {
    return std::shared_ptr<LogSink>(new LogSink());
  }

  LogSink::LogSink()
    : m_filter_log_level(spdlog::level::trace)
    , m_number_of_log_entries(0)
    , m_indices_in_bytes(0)
    , m_enable_auto_scroll(true)
    , m_previous_filter_level(spdlog::level::trace)
    , m_previous_filter_text{}
  {}
  LogSink::~LogSink() = default;

  auto LogSink::draw_debug_frame() -> void
  {
    auto const& io = im::GetIO();
    im::SetNextWindowSize({800, 440}, ImGuiCond_FirstUseEver);
    im::SetNextWindowPos({io.DisplaySize.x - 805, 5}, ImGuiCond_FirstUseEver);
    if(im::Begin("Debug Console")) {
      if(im::BeginPopup("Options"))
        im::OpenPopup("Options");
      im::SameLine();
      im::TextDisabled("(?)");
      if(im::IsItemHovered()) {
        im::BeginTooltip();
        im::PushTextWrapPos(im::GetFontSize() * 35.0f);
        im::Text("%s", "Clearing the log is disabled due to a memory corruption bug.");
        im::PopTextWrapPos();
        im::EndTooltip();
      }
      im::SameLine();
      im::BeginDisabled();
      if(im::Button("Clear"))
        this->clear_log_buffers();
      im::EndDisabled();
      im::SameLine();
      im::Checkbox("Auto scroll", &this->m_enable_auto_scroll);
      im::Text("%d messages logged, using %dMB of memory",
               this->m_number_of_log_entries,
               (this->m_logged_content.size() + this->m_indices_in_bytes) / 1024 / 1024
      );

      static auto const level_names = vector SPDLOG_LEVEL_NAMES;
      static auto level_names_chars = [&]() -> vector<char const*> {
        vector<char const*> chars;
        chars.reserve(level_names.size());
        for(auto const& level_name : level_names)
          chars.push_back(level_name.data());
        return chars;
      }();
      auto log_selection_width = [&]() -> f32 {
        auto longest_text_width = 0.0f;
        for(auto log_level_text : level_names) {
          auto text_width = im::CalcTextSize(log_level_text.data()).x;
          longest_text_width = std::max(text_width, longest_text_width);
        }
        return longest_text_width + im::GetStyle().FramePadding.x * 2.0f + im::GetFrameHeight();
      }();
      auto combobox_right_alignment = im::GetWindowSize().x - (log_selection_width + im::GetStyle().WindowPadding.x);
      auto active_log_level = spdlog::get_level();
      im::SetNextItemWidth(log_selection_width);
      im::SameLine(combobox_right_alignment);
      im::Combo(
        "##ActiveLogLevel",
        reinterpret_cast<i32*>(&active_log_level),
        level_names_chars.data(),
        static_cast<int>(level_names_chars.size())
      );
      if(active_log_level != spdlog::get_level())
        spdlog::set_level(active_log_level);

      this->m_filter_text_match.Draw(
        "##LogFilter",
        im::GetWindowSize().x - (log_selection_width + im::GetStyle().WindowPadding.x * 2.0f + im::GetStyle().FramePadding.x)
      );
      im::SetNextItemWidth(log_selection_width);
      im::SameLine(combobox_right_alignment);
      im::Combo(
        "##FilterLogLevel",
        &this->m_filter_log_level,
        level_names_chars.data(),
        static_cast<int>(level_names_chars.size())
      );
      im::Separator();
      im::BeginChild("LogTextView", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
      im::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

      auto const lock = std::lock_guard(spdlog::sinks::base_sink<std::mutex>::mutex_);
      this->rebuild_filter_with_previous_states();
      auto view_clipper = ImGuiListClipper();
      view_clipper.Begin(this->m_filtered_view.size());
      while(view_clipper.Step()) {
        auto styles_pushed_to_stack = 0;
        for(auto clipper_line_number = view_clipper.DisplayStart;
             clipper_line_number < view_clipper.DisplayEnd;
             ++clipper_line_number) {
          auto& log_metadata_entry = this->m_log_metadata[this->m_filtered_view[clipper_line_number]];
          if(log_metadata_entry.level == spdlog::level::n_levels)
            im::Indent();
          for(auto i = 0; i < log_metadata_entry.format_ranges.size(); ++i) {
            switch(log_metadata_entry.format_ranges[i].format_tag) {
              case ANSIColors::Reset:
                im::PopStyleColor(styles_pushed_to_stack);
                styles_pushed_to_stack = 0;
                break;
              case ANSIColors::Black: [[fallthrough]];
              case ANSIColors::Red: [[fallthrough]];
              case ANSIColors::Green: [[fallthrough]];
              case ANSIColors::Yellow: [[fallthrough]];
              case ANSIColors::Blue: [[fallthrough]];
              case ANSIColors::Magenta: [[fallthrough]];
              case ANSIColors::Cyan: [[fallthrough]];
              case ANSIColors::White: {
                static ImVec4 const basic_colors_to_vec[] = {
                  { 0.0f, 0.0f, 0.0f, 1.0f },       // "black"
                  { 1.0f, 0.333f, 0.333f, 1.0f },   // #FF5555
                  { 0.314f, 0.98f, 0.482f, 1.0f },  // #50FA7B
                  { 0.945f, 0.98f, 0.549f, 1.0f },  // #F1FA8C
                  { 0.545f, 0.914f, 0.992f, 1.0f }, // #8BE9FD
                  { 1.0f, 0.475f, 0.776f, 1.0f },   // #FF79C6
                  { 0.545f, 0.914f, 0.992f, 1.0f }, // #8BE9FD
                  { 0.267f, 0.278f, 0.353f, 1.0f }  // #44475A
                };
                im::PushStyleColor(
                  ImGuiCol_Text,
                  basic_colors_to_vec[log_metadata_entry.format_ranges[i].format_tag - ANSIColors::Black]
                );
                ++styles_pushed_to_stack;
                break;
              }
              case ANSIColors::BrightBlack: [[fallthrough]];
              case ANSIColors::BrightRed: [[fallthrough]];
              case ANSIColors::BrightGreen: [[fallthrough]];
              case ANSIColors::BrightYellow: [[fallthrough]];
              case ANSIColors::BrightBlue: [[fallthrough]];
              case ANSIColors::BrightMagenta: [[fallthrough]];
              case ANSIColors::BrightCyan: [[fallthrough]];
              case ANSIColors::BrightWhite: {
                static ImVec4 const basic_colors_to_vec[] = {
                  { 0.0f, 0.0f, 0.0f, 1.0f }, // COLOR_BRIGHTBLACK
                  { 1.0f, 0.0f, 0.0f, 1.0f }, // COLOR_BRIGHTRED
                  { 0.0f, 1.0f, 0.0f, 1.0f }, // COLOR_BRIGHTGREEN
                  { 1.0f, 1.0f, 0.0f, 1.0f }, // COLOR_BRIGHTYELLOW
                  { 0.0f, 0.0f, 1.0f, 1.0f }, // COLOR_BRIGHTBLUE
                  { 1.0f, 0.0f, 1.0f, 1.0f }, // COLOR_BRIGHTMAGENTA
                  { 0.0f, 1.0f, 1.0f, 1.0f }, // COLOR_BRIGHTCYAN
                  { 1.0f, 1.0f, 1.0f, 1.0f }  // COLOR_BRIGHTWHITE
                };
                im::PushStyleColor(
                  ImGuiCol_Text,
                  basic_colors_to_vec[log_metadata_entry.format_ranges[i].format_tag - ANSIColors::BrightBlack]
                );
                ++styles_pushed_to_stack;
                break;
              }
              default: break;
            }
            auto const ref_ptr = this->m_logged_content.begin() + log_metadata_entry.base_offset;
            im::Text("%s", detail::make_null_terminated(
              ref_ptr + log_metadata_entry.format_ranges[i].substring_begin,
              ref_ptr + log_metadata_entry.format_ranges[i].substring_end
            ).c_str());
            if(log_metadata_entry.format_ranges.size() - (i + 1))
              im::SameLine();
          }
          if(log_metadata_entry.level == spdlog::level::n_levels)
            im::Unindent();
        }
        im::PopStyleColor(styles_pushed_to_stack);
      }
      view_clipper.End();
      im::PopStyleVar();
      if(this->m_enable_auto_scroll and im::GetScrollY() >= im::GetScrollMaxY())
        im::SetScrollHereY(1.0f);
      im::EndChild();
    }
    im::End();
  }

  auto LogSink::clear_log_buffers(bool disable_lock) -> void {
    if(not disable_lock)
      spdlog::sinks::base_sink<std::mutex>::mutex_.lock();
    this->m_logged_content.clear();
    this->m_log_metadata.clear();
    this->m_number_of_log_entries = 0;
    this->m_indices_in_bytes = 0;
    if(not disable_lock)
      spdlog::sinks::base_sink<std::mutex>::mutex_.unlock();
  }

  auto LogSink::sink_it_(const spdlog::details::log_msg& msg) noexcept -> void
  {
    ++this->m_number_of_log_entries;
    auto formatted_buffer = spdlog::memory_buf_t{};
    spdlog::sinks::base_sink<std::mutex>::formatter_->format(msg, formatted_buffer);
    auto formatted_text = fmt::to_string(formatted_buffer);

    static auto constexpr color_to_escape = std::array {
      "\x1b[37m",
      "\x1b[34m",
      "\x1b[32m",
      "\x1b[33m",
      "\x1b[31m",
      "\x1b[91m",
    };
    formatted_text.insert(msg.color_range_start, color_to_escape[msg.level]);
    formatted_text.insert(msg.color_range_end + 5, "\x1b[0m");
    auto filter_passing = msg.level >= this->m_filter_log_level;
    filter_passing and_eq this->m_filter_text_match.PassFilter(formatted_text.c_str(), formatted_text.c_str() + formatted_text.size());
    auto old_text_buf_size = this->m_logged_content.size();
    auto message_data2 = SinkLineContent {
      .level = msg.level,
      .base_offset = old_text_buf_size
    };
    auto last_sequence_tag_since_begin = ANSIColors::Reset;
    switch(formatted_text[0]) {
      case '\x1b': [[fallthrough]];
      case '\n': break;
      default:
        message_data2.format_ranges.push_back({ 0, 0, last_sequence_tag_since_begin });
        break;
    }
    for(auto i = 0; i < formatted_text.size(); ++i)
      switch(formatted_text[i]) {
        case '\n': {
          if(not message_data2.format_ranges.empty())
            message_data2.format_ranges.back().substring_end = i;
          if(filter_passing)
            this->m_filtered_view.push_back(static_cast<int>(this->m_log_metadata.size()));
          this->m_log_metadata.push_back(message_data2);
          this->m_indices_in_bytes += message_data2.format_ranges.size() *
                                        sizeof(SinkLineContent::ColorDataRanges) + sizeof(SinkLineContent);
          message_data2.level = spdlog::level::n_levels;
          message_data2.base_offset = old_text_buf_size + i + 1;
          message_data2.format_ranges.clear();
          message_data2.format_ranges.push_back({static_cast<u32>(i + 1), 0, last_sequence_tag_since_begin});
        }
        break;
        case '\x1b': {
          auto position_processed = 0UZ;
          auto escape_sequence_code = std::stoi(&formatted_text[i + 2], &position_processed);
          auto substr = formatted_text.substr(i + 2);
          ++position_processed;
          last_sequence_tag_since_begin = static_cast<ANSIColors>(escape_sequence_code);
          auto fmt = SinkLineContent::ColorDataRanges { static_cast<u32>(i), 0, static_cast<u32>(escape_sequence_code) };
          if(not message_data2.format_ranges.empty())
            message_data2.format_ranges.back().substring_end = fmt.substring_begin;
          message_data2.format_ranges.push_back(fmt);
          formatted_text.erase(formatted_text.begin() + i--, formatted_text.begin() + (i + 2 + position_processed));
        }
        break;
        default:
          break;
      }
    this->m_logged_content.append(formatted_text.c_str(), formatted_text.c_str() + formatted_text.size());
  }

  auto LogSink::flush_() -> void {}

  auto LogSink::rebuild_filter_with_previous_states() -> void
  {
    auto rebuild_type = static_cast<i32>(this->m_previous_filter_level - this->m_filter_log_level);
    rebuild_type or_eq std::memcmp(
      this->m_previous_filter_text,
      this->m_filter_text_match.InputBuf,
      sizeof(this->m_previous_filter_text)
    );
    if(rebuild_type)
    {
      auto new_line_passes = false;
      this->m_filtered_view.clear();

      for(auto i = 0; i < this->m_log_metadata.size(); ++i) {
        if(this->m_log_metadata[i].level == spdlog::level::n_levels) {
          if(new_line_passes)
            this->m_filtered_view.push_back(static_cast<int>(i));
          continue;
        }
        if(this->m_log_metadata[i].level < this->m_filter_log_level) {
          new_line_passes = false;
          continue;
        }
        auto line_begin = this->m_logged_content.begin()
                          + this->m_log_metadata[i].base_offset
                          + this->m_log_metadata[i].format_ranges.front().substring_begin;
        auto line_end = this->m_logged_content.begin()
                        + this->m_log_metadata[i].base_offset
                        + this->m_log_metadata[i].format_ranges.front().substring_end;
        if(not this->m_filter_text_match.PassFilter(line_begin, line_end)) {
          new_line_passes = false;
          continue;
        }
        new_line_passes = true;
        this->m_filtered_view.push_back(static_cast<int>(i));
      }
    }
    this->m_previous_filter_level = this->m_filter_log_level;
    std::memcpy(this->m_previous_filter_text, this->m_filter_text_match.InputBuf, sizeof(this->m_previous_filter_text));
  }
} // namespace corona::standalone::gui::immediate
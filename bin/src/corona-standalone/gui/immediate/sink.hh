#pragma once

#include <vector>
#include <spdlog/sinks/base_sink.h>
#include <corona-standalone/definitions.hh>
#include <corona-standalone/gui/immediate/drawable.hh>
#include <corona-standalone/gui/immediate/extensions.hh>

namespace corona::standalone::gui::immediate
{
  using std::vector;
  using std::shared_ptr;

  class LogSink : public spdlog::sinks::base_sink<std::mutex>,
                  public std::enable_shared_from_this<LogSink>,
                  public Drawable
  {
    class SinkLineContent
    {
     public:
      spdlog::level::level_enum level;
      i32 base_offset;

      struct ColorDataRanges
      {
        u32 substring_begin : 12;
        u32 substring_end   : 12;
        u32 format_tag      : 8;
      };
      im::vector<ColorDataRanges> format_ranges;
    };

   public:
    [[nodiscard("cannot discard result of factory function call")]]
    static auto create() noexcept -> shared_ptr<LogSink>;

    ~LogSink() override;

    auto draw_debug_frame() -> void override;

   protected:
    LogSink();

    auto sink_it_(spdlog::details::log_msg const& msg) noexcept -> void override;
    auto flush_() -> void override;

   private:
    auto clear_log_buffers(bool disable_lock = false) -> void;
    auto rebuild_filter_with_previous_states() -> void;

   private:
    im::text_buffer m_logged_content;
    vector<SinkLineContent> m_log_metadata;
    im::text_filter m_filter_text_match;
    im::vector<i32> m_filtered_view;
    i32 m_filter_log_level;
    u32 m_number_of_log_entries;
    u32 m_indices_in_bytes;
    bool m_enable_auto_scroll;
    i32 m_previous_filter_level;
    decltype(im::text_filter::InputBuf) m_previous_filter_text;
  };
} // namespace corona::standalone::gui::immediate
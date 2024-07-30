#pragma once

#include <atomic>
#include <chrono>
#include <boost/asio.hpp>
#include <corona/detail/export.h>
#include <corona/vendored/imgui.h>
#include <private/misc/rotating_debug_buffer.hh>

namespace corona::misc
{
  template <usize N, typename T>
  struct socket_debug
  {
    enum Direction
    {
      In,
      Out
    };

    enum class PlotType : char
    {
      Histogram,
      Line
    };

    socket_debug(boost::asio::io_context& ctx, std::chrono::steady_clock::duration interval)
      : worker_(ctx, interval)
      , interval_(interval) {
      this->work();
    }

    ~socket_debug() {
      this->running_ = false;
      this->worker_.cancel();
    }

    inline auto update(Direction dir, usize data) -> void {
      if(dir == In)
        this->in_.push(static_cast<T>(data));
      else
        this->out_.push(static_cast<T>(data));
    }

    inline auto operator()(Direction dir, usize data) -> void {
      this->update(dir, data);
    }

    inline auto plot(
      Direction dir,
      PlotType plot_type,
      std::string_view title,
      f32 size_x = ImGui::GetContentRegionAvail().x,
      f32 size_y = 60
    ) const -> void {
      auto* data = dir == Direction::In ? this->in_.buf.data() : this->out_.buf.data();
      auto size = static_cast<int>(dir == Direction::In ? this->in_.buf.size() : this->out_.buf.size());
      switch(plot_type) {
        case PlotType::Histogram:
          vendored::imgui::PlotHistogram(
            "",
            data,
            size,
            0,
            title.data(),
            0.0f,
            std::numeric_limits<f32>::max(),
            ImVec2(size_x, size_y)
          );
          break;
        case PlotType::Line:
          vendored::imgui::PlotLines(
            "",
            data,
            size,
            0,
            title.data(),
            0.0f,
            std::numeric_limits<f32>::max(),
            ImVec2(size_x, size_y)
          );
          break;
      }
    }

    inline auto plot_all(PlotType plot_type, f32 size_y = 60, bool same_line = false) const -> void {
      this->plot(
        In,
        plot_type,
        "Incoming bytes",
        (not same_line) ? ImGui::GetContentRegionAvail().x : ImGui::GetContentRegionAvail().x / 2.0f,
        size_y
      );
      if(same_line)
        vendored::imgui::SameLine();
      this->plot(
        Out,
        plot_type,
        "Outgoing bytes",
        ImGui::GetContentRegionAvail().x,
        size_y
      );
    }

   private:
    inline auto work() -> void {
      if(not this->running_)
        return;
      this->worker_.expires_from_now(this->interval_);
      this->worker_.async_wait([this](auto const& ec) {
        if(not ec) {
          this->in_.rotate();
          this->out_.rotate();
        }
        this->work();
      });
    }

    rotating_debug_buffer<N, T> in_ {};
    rotating_debug_buffer<N, T> out_ {};
    boost::asio::steady_timer worker_;
    std::chrono::steady_clock::duration interval_;
    std::atomic<bool> running_ {false};
  };
} // namespace corona::misc
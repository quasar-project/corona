#pragma once

#include <atomic>
#include <string>
#include <boost/asio.hpp>

namespace corona::misc
{
  template <std::invocable F>
  struct call_every
  {
   public:
    call_every(boost::asio::io_context& ctx, std::chrono::steady_clock::duration interval, F&& f)
      : f(std::forward<F>(f))
      , timer(ctx, interval)
      , call_interval(interval)
    {
      this->work();
    }

    ~call_every() {
      this->running = false;
      this->timer.cancel();
    }

    [[nodiscard]] inline auto interval() const -> std::chrono::steady_clock::duration {
      return this->call_interval;
    }

    auto stop() -> void {
      this->running = false;
      this->timer.cancel();
    }

    auto restart() -> void {
      this->stop();
      this->running = true;
      this->work();
    }

   private:
    inline auto work() -> void {
      if(not this->running)
        return;
      this->timer.expires_from_now(this->interval());
      this->timer.async_wait([this](auto const& ec){
        if(not ec)
          this->f();
        this->work();
      });
    }

    F f;
    std::atomic<bool> running = true;
    boost::asio::steady_timer timer;
    std::chrono::steady_clock::duration call_interval;
  };
} // namespace corona::misc
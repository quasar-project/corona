#pragma once

#include <qdebug.h>
#include <floppy/traits.h>
#include <corona-standalone/app/logger.hh>

namespace corona::standalone::app
{
  class UILogger : public fl::traits::singleton<UILogger>
  {
    friend class fl::traits::singleton<UILogger>;

   public:
    [[nodiscard]] auto logger() const -> spdlog::logger& {
      return **this->logger_;
    }

    static auto message_handler(
      ::QtMsgType type,
      [[maybe_unused]] ::QMessageLogContext const& ctx,
      ::QString const& msg
    ) -> void {
      if(not spdlog::default_logger())
        return;
      switch(type) {
        case ::QtDebugMsg: fl::log::debug_to(UILogger::ref_mut().logger(), "{}", msg); break;
        case ::QtInfoMsg: fl::log::info_to(UILogger::ref_mut().logger(), "{}", msg); break;
        case ::QtWarningMsg: fl::log::warn_to(UILogger::ref_mut().logger(), "{}", msg); break;
        case ::QtCriticalMsg: fl::log::error_to(UILogger::ref_mut().logger(), "{}", msg); break;
        case ::QtFatalMsg: fl::log::critical_to(UILogger::ref_mut().logger(), "{}", msg); break;
        [[unlikely]] default: fl::unreachable();
      }
    }

   private:
    Logger logger_ = Logger::make(
      Logger::Params {
        .is_default = false,
        .name = "ui",
        .level = llog::level::level_enum::trace,
        .target = Logger::Target::All,
        .pattern = "[%X] (%n) [%^%L%$] %^%v%$",
        .filename = "corona_ui.log",
        .folder = std::filesystem::current_path() / "logs" / "ui"
      }
    );
  };
} // namespace corona::standalone::app
 

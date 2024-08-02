#pragma once

#include <qdebug.h>
#include <floppy/traits.h>
#include <corona-standalone/app/class_logger.hh>

namespace corona::standalone::app
{
  class CUserInterfaceLogger : public fl::singleton<CUserInterfaceLogger>
  {
    friend class fl::singleton<CUserInterfaceLogger>;

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
        case ::QtDebugMsg: fl::log::debug_to(CUserInterfaceLogger::ref_mut().logger(), "{}", msg); break;
        case ::QtInfoMsg: fl::log::info_to(CUserInterfaceLogger::ref_mut().logger(), "{}", msg); break;
        case ::QtWarningMsg: fl::log::warn_to(CUserInterfaceLogger::ref_mut().logger(), "{}", msg); break;
        case ::QtCriticalMsg: fl::log::error_to(CUserInterfaceLogger::ref_mut().logger(), "{}", msg); break;
        case ::QtFatalMsg: fl::log::critical_to(CUserInterfaceLogger::ref_mut().logger(), "{}", msg); break;
        [[unlikely]] default: fl::unreachable();
      }
    }

    auto install() -> void {
      this->prev_handler_ = ::qInstallMessageHandler(CUserInterfaceLogger::message_handler);
      llog::debug("CUserInterfaceLogger: installed QtMessageHandler");
    }

    auto uninstall() -> void {
      ::qInstallMessageHandler(this->prev_handler_);
      llog::debug("CUserInterfaceLogger: uninstalled QtMessageHandler");
    }

   private:
    ::QtMessageHandler prev_handler_ = nullptr;
    CLogger logger_ = CLogger::make(
      CLogger::Params {
        .is_default = false,
        .name = "ui",
        .level = llog::level::trace,
        .target = CLogger::Target::All,
        .pattern = "[%X] (%n) [%^%L%$] %^%v%$",
        .filename = "corona_ui.log",
        .folder = std::filesystem::current_path() / "logs" / "ui"
      }
    );
  };
} // namespace corona::standalone::app
 

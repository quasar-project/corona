#include <corona-standalone/app/logger.hh>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <magic_enum/magic_enum.hpp>

using std::vector;
namespace me = magic_enum;

namespace corona::standalone::app
{
  Logger::Logger(Params params)
    : params_(std::move(params))
  {
    if(this->params().pattern.empty())
      llog::warn()("no pattern specified for logger \'{}\'. default pattern will be used", this->params().name);
    auto sinks = vector<spdlog::sink_ptr>();
    auto bitmask = fl::to_underlying(this->params().target);
    auto mask = static_cast<char>(1);
    auto const folder = [this]() {
      if(not this->params().folder.has_value()) {
        llog::warn()("no folder specified for logger \'{}\'. assuming current directory", this->params().name);
        return fs::current_path();
      }
      return *this->params().folder;
    }();
    if(not exists(folder)) {
      llog::trace()("log folder provided for logger \'{}\' does not exist. creating", this->params().name);
      create_directories(folder);
    }
    while(bitmask) {
      switch(bitmask bitand mask) {
        case fl::to_underlying(Target::File):
          if(not this->params().filename.has_value())
            fl::panic(fmt::format("no filename specified for logger \'{}\'", this->params().name));
          sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            (folder / *this->params().filename).string(),
            this->params().max_file_size_mb * 1024 * 1024,
            this->params().max_files)
          );
          break;
        case fl::to_underlying(Target::Stdout):
          sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
          break;
        default: fl::unreachable();
      }
      bitmask &= ~mask;
      mask <<= 1;
    }
    this->logger_ = std::make_shared<spdlog::logger>(this->params().name, sinks.begin(), sinks.end());
    if(this->params().is_default)
      spdlog::set_default_logger(**this);
    (**this)->set_level(this->params().level);
    if(not this->params().pattern.empty())
      (**this)->set_pattern(this->params().pattern);
    (**this)->flush_on(llog::level::level_enum::debug);
    spdlog::flush_every(std::chrono::seconds(1));
    this->logger_ = **this;
    llog::trace()(R"(logger '{}': created with level '{}' and target '{}')",
      this->get()->name(),
      spdlog::level::to_string_view(this->params().level),
      string(me::enum_name(this->params().target))
    );
    if(this->params().filename.has_value()) {
      llog::trace()(R"(logger '{}': working directory is '{}')", this->params().name, folder.generic_string());
      llog::trace()(R"(logger '{}': filename = '{}', max file size = {} MB, max files = {})",
        this->params().name,
        *this->params().filename,
        this->params().max_file_size_mb,
        this->params().max_files
      );
    }
    (**this)->flush();
  }

  Logger::~Logger() {
    if(not *this)
      return;
    llog::trace()("logger \'{}\': closing", this->get()->name());
    if(this->params().is_default)
      spdlog::shutdown();
  }

  auto Logger::make(const Logger::Params& params) -> Logger { return Logger(params); }
} // namespace corona::standalone::app
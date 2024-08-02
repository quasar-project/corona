#pragma once

#include <memory>
#include <filesystem>
#include <floppy/logging.h>
#include <floppy/traits.h>
#include <corona-standalone/definitions.hh>

namespace corona::standalone::app
{
  using std::shared_ptr;
  namespace fs = std::filesystem;

  struct CLogger : public fl::pin // NOLINT(*-special-member-functions) (reason: pinned)
  {
   public:
    enum class Target : char
    {
      Stdout = 1 << 0,
      File   = 1 << 1,
      All    = Stdout | File
    };

    struct Params
    {
      bool is_default;
      string name;
      llog::level::level_enum level;
      Target target;
      string pattern;
      option<string> filename;
      option<fs::path> folder;
      f32 max_file_size_mb = 10.0F;
      usize max_files = 3;
    };

    explicit CLogger(Params params);
    ~CLogger();

    [[nodiscard]] auto get() -> shared_ptr<spdlog::logger>& { return this->logger_; }
    [[nodiscard]] auto get() const -> shared_ptr<spdlog::logger> const& { return this->logger_; }
    [[nodiscard]] auto params() const -> Params const& { return this->params_; }

    [[nodiscard]] explicit operator bool() const { return this->logger_ != nullptr; }
    [[nodiscard]] auto operator*() -> shared_ptr<spdlog::logger>& { return this->logger_; }
    [[nodiscard]] auto operator*() const -> shared_ptr<spdlog::logger> const& { return this->logger_; }
    [[nodiscard]] auto operator->() -> shared_ptr<spdlog::logger>& { return this->logger_; }
    [[nodiscard]] auto operator->() const -> shared_ptr<spdlog::logger> const& { return this->logger_; }

    [[nodiscard]] static auto make(Params const& params) -> CLogger;

   private:
    shared_ptr<spdlog::logger> logger_;
    Params params_;
  };

  constexpr auto operator|(CLogger::Target const& lhs, CLogger::Target const& rhs) -> CLogger::Target {
    return static_cast<CLogger::Target>(static_cast<char>(lhs) | static_cast<char>(rhs));
  }

  constexpr auto operator&(CLogger::Target const& lhs, CLogger::Target const& rhs) -> CLogger::Target {
    return static_cast<CLogger::Target>(static_cast<char>(lhs) & static_cast<char>(rhs));
  }
} // namespace corona::standalone::app
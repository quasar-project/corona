#pragma once

#include <algorithm>
#include <qurl.h>
#include <qobject.h>
#include <qqml.h>
#include <qqmlmoduleregistration.h>
#include <fmt/color.h>
#include "detail/export.h"

namespace corona::modules::qmlbind
{
  namespace detail
  {
    [[nodiscard]] auto strip_prefix_and_extension(std::string_view url) -> result<std::string> {
      auto const last_slash = url.find_last_of('/');
      if(last_slash == std::string_view::npos)
        return error("failed to strip prefix from url: {}", url);
      auto const last_dot = url.find_last_of('.');
      if(last_dot == std::string_view::npos)
        return error("failed to strip extension from url: {}", url);
      return std::string(url.substr(last_slash + 1, last_dot - last_slash - 1));
    }

    [[nodiscard]] auto strip_namespace(std::string_view name) -> std::string {
      auto const last_colon = name.find_last_of(':');
      if(last_colon == std::string_view::npos)
        return std::string(name);
      return std::string(name.substr(last_colon + 1));
    }

    [[nodiscard]] auto strip_hungarian_prefix(std::string_view name) -> std::string {
      if(name.size() < 2)
        return std::string(name);
      if(name[0] == 'C' and std::isupper(name[1]))
        return std::string(name.substr(1));
      return std::string(name);
    }
  } // namespace detail

  enum class verbosity : char
  {
    quiet,
    verbose
  };

  #ifdef FL_DEBUG
  # define __qmlbind_implicit_verbosity_level__ verbosity::verbose // NOLINT(*-reserved-identifier)
  #else // FL_DEBUG
  # define __qmlbind_implicit_verbosity_level__ verbosity::quiet // NOLINT(*-reserved-identifier)
  #endif // FL_DEBUG

  template <auto Verbosity = __qmlbind_implicit_verbosity_level__>
  requires std::is_same_v<decltype(Verbosity), verbosity>
  class module
  {
   public:
    using version_type = fl::meta::version;
    using verbosity_type = decltype(Verbosity);

    explicit module(std::string name, version_type version = version_type(1, 0, 0)) noexcept
      : name_(std::move(name))
      , version_(version) {
      if constexpr(Verbosity == verbosity::verbose)
        fmt::println("qmlbind: registering module {} v{}.{}",
          fmt::styled(this->name_, fmt::fg(fmt::terminal_color::blue) | fmt::emphasis::bold),
          fmt::styled(this->version_.major(), fmt::fg(fmt::terminal_color::cyan)),
          fmt::styled(this->version_.minor(), fmt::fg(fmt::terminal_color::cyan))
        );
      ::qmlRegisterModule(this->name_.c_str(), this->version_.major(), this->version_.minor());
    }

    template <std::derived_from<::QObject> T>
    auto component(option<std::string_view> name = none) -> module& {
      auto const component_name = [&name]() -> std::string {
        if(name)
          return std::string(*name);
        auto const meta_name = detail::strip_namespace(T::staticMetaObject.className());
        return detail::strip_hungarian_prefix(meta_name);
      }();
      if constexpr(Verbosity == verbosity::verbose)
        fmt::println("qmlbind: \tregistering type {} (qobject)",
          fmt::styled(component_name, fmt::fg(fmt::terminal_color::magenta) | fmt::emphasis::bold)
        );
      ::qmlRegisterType<T>(this->name_.c_str(), this->version_.major(), this->version_.minor(), component_name.c_str());
      return *this;
    }

    auto qml_file(std::string_view url, option<std::string_view> name = none) -> module& {
      auto const component_name = [&name , url]() -> std::string {
        if(name)
          return std::string(*name);
        if(auto const res = detail::strip_prefix_and_extension(url); res)
          return { *res };
        else {
          if constexpr(Verbosity == verbosity::verbose)
            fmt::println(stderr, "qmlbind: error during qml registration: {}",
              fmt::styled(res.error(), fmt::fg(fmt::terminal_color::red) | fmt::emphasis::bold)
            );
          return "Unknown";
        }
      }();
      if constexpr(Verbosity == verbosity::verbose)
        fmt::println("qmlbind: \tregistering type {} from {}",
          fmt::styled(component_name, fmt::fg(fmt::terminal_color::yellow) | fmt::emphasis::bold),
          fmt::styled(url, fmt::emphasis::faint)
        );
      ::qmlRegisterType(::QUrl(url.data()), this->name_.c_str(), this->version_.major(), this->version_.minor(), component_name.c_str());
      return *this;
    }

   private:
    std::string name_;
    version_type version_;
  };
} // namespace corona::modules::qmlbind
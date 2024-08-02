#pragma once

#include <functional>
#include "export.h"

namespace corona::modules::imgui::detail
{
  [[maybe_unused]] static inline auto vec2_zero = vec2(.0F, .0F);

  template <typename T, bool ForceDestructorCall = false>
  struct scoped_template
  {
    using wrapped_type = T;
    using self_type = scoped_template<T>;

   protected:
    bool ok_;

   public:
    constexpr scoped_template(bool ok) noexcept
      : ok_(ok)
    {}

    scoped_template(scoped_template const&) = delete;
    scoped_template(scoped_template&&) = delete;
    auto operator=(scoped_template const&) -> scoped_template& = delete;
    auto operator=(scoped_template&&) -> scoped_template& = delete;

    ~scoped_template() noexcept {
      if constexpr(not ForceDestructorCall)
        if(not this->ok_)
          return;
      T::dtor();
    }

    template <std::invocable F>
//    requires requires {
//      { F{}() } -> std::same_as<void>;
//    }
    constexpr auto operator&&(F&& func) const noexcept -> bool {
      if(this->ok_)
        func();
      return this->ok_;
    }

    constexpr explicit operator bool() const noexcept { return this->ok_; }
  };
} // namespace corona::modules::imgui::detail
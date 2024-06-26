#pragma once

#include <stdexcept>
#include <filesystem>
#include <corona/detail/export.h>

namespace corona::image
{
  /// \brief Image decoding exception class.
  /// \details Thrown when an error occurs while decoding an image either from
  /// file or from memory.
  class decoding_error : public std::runtime_error
  {
    public:
      /// \brief Construct a new decoding error object with file and reason message.
      /// \param file File in which the error occurred.
      /// \param reason Reason of the error.
      decoding_error(
        std::filesystem::path const& file,
        std::string_view reason
      )
        : std::runtime_error(fmt::format("decoding error: in file '{}': {}", file.string(), reason))
      {}

      /// \brief Construct a new decoding error object with reason message.
      /// \param reason Reason of the error.
      explicit decoding_error(std::string_view reason)
        : std::runtime_error(fmt::format("decoding error: {}", reason))
      {}
  };
} // namespace corona::image
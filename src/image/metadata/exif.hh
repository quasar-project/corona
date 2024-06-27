#pragma once

#include <bit>
#include <corona/image/error.h>

namespace corona::image::metadata
{
  /// \brief JPEG metadata header. Serialized in big endian.
  struct ExifHeader
  {
    /// \brief Decodes header from a pointer to bytes containing the header.
    /// \param data Pointer to bytes containing the header.
    /// \warning This functions assumes the pointer is valid. If you want more secure functions,
    /// use \ref from_bytes instead.
    explicit ExifHeader(byte const* data)
      : marker(std::bit_cast<ExifHeader const*>(data)->marker)
      , size(std::bit_cast<ExifHeader const*>(data)->size)
    {
      if constexpr(std::endian::native != std::endian::big) {
        this->marker = fl::platform::swap_endian(this->marker);
        this->size = fl::platform::swap_endian(this->size);
      }
    }

    /// \brief Decodes header from a pointer to bytes containing the header.
    /// \details Caller is responsible for pointer validity. If pointer points outside the
    /// allocated memory, the behaviour is undefined.
    /// In case the caller can't ensure pointer validity, consider using overload which decodes from
    /// byte span.
    /// \param data Pointer to bytes containing the header.
    /// \throws corona::image::decoding_error If the pointer is null.
    [[nodiscard]] static auto from_bytes(byte const* data) -> ExifHeader {
      if(data == nullptr)
        throw decoding_error("exif data is null");
      return ExifHeader(data);
    }

    /// \brief Decodes header from a span of bytes containing the header.
    /// \details Span must be at least <code>sizeof(ExifHeader)</code> bytes long, otherwise the
    /// function will throw a \ref corona::image::decoding_error.
    /// \param data Span of bytes containing the header.
    /// \throws corona::image::decoding_error If the span is too small.
    [[nodiscard]] static auto from_bytes(span<byte const> const data) -> ExifHeader {
      if(data.size_bytes() < sizeof(ExifHeader))
        throw decoding_error("exif data too small");
      return ExifHeader::from_bytes(data.data());
    }

    u16 marker;
    u16 size;
  };
} // namespace corona::image::metadata
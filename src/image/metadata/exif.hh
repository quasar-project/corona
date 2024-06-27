#pragma once

#include <corona/image/error.h>
#include <utility/memory.hh>

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

  /// \brief Exif metadata contents.
  /// \details Serialized in little endian.
  struct ExifMetadata
  {
    f64 latitude;        ///< Latitude in WGS84 datum of image anchor point (°)
    f64 longitude;       ///< Longitude in WGS84 datum of image anchor point (°)
    f32 dx;              ///< Horizontal resolution (m/pixel)
    f32 dy;              ///< Vertical resolution (m/pixel)
    f32 x0;              ///< Near edge of image offset (m)
    f32 y0;              ///< Frame offset (m)
    f32 azimuth;         ///< Azimuth of image (°)
    f32 drift_angle;     ///< Drift angle relative to azimuth (°)
    f32 lx;              ///< Image width (m)
    f32 ly;              ///< Image height (m)
    f32 div;             ///< Arc divergence (°)
    f32 velocity;        ///< Velocity in the moment of capture (km/h)
    f32 altitude;        ///< Altitude in the moment of capture rel. to sea level (m)
    f32 fic;             ///< Frequency Interpolation Coefficient
    f32 time_offset;     ///< Image offset from time of capture (s)
    f32 time_duration;   ///< Total capture duration (s)
    //NOLINTNEXTLINE(*-avoid-c-arrays)
    [[maybe_unused]] byte reserved_1[8]; ///< Reserved 8 bytes (1)
    byte mode;           ///< Image mode
    byte image_type;     ///< Image type (0 means telescopic)
    //NOLINTNEXTLINE(*-avoid-c-arrays)
    [[maybe_unused]] byte reserved_3[8]; ///< Reserved 8 bytes (2)
    u16 checksum;        ///< Checksum (CRC16)

    [[nodiscard]] static auto from_bytes(span<byte const> const data) -> ExifMetadata {
      if(data.size_bytes() < sizeof(ExifMetadata))
        throw decoding_error("exif data too small");
      return utility::memory::byte_cast<ExifMetadata>(data.data());
    }
  };
} // namespace corona::image::metadata
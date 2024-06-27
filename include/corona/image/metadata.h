#pragma once

#include <span>
#include <filesystem>
#include <floppy/euclid.h>
#include <floppy/traits.h>
#include <corona/detail/export.h>
#include <corona/image/error.h>

namespace corona::image
{
  using std::span;
  using std::byte;
  namespace fs = std::filesystem;

  /// \brief Image metadata generic class.
  /// \details Provides functions to read metadata from different formats: EXIF, JSON, etc.
  class Metadata : public fl::traits::formattable<Metadata, char>
  {
   public:
    /// \brief Exif decode options.
    /// \details Options for the EXIF decoder. See \ref corona::image::Metadata::from_exif_file
    /// and \ref corona::image::Metadata::from_exif_data.
    struct ExifDecodeOptions
    {
      /// \brief Default constructor.
      ExifDecodeOptions()
        : max_metadata_size(1024)
        , header_offset(20)
        , metadata_marker(0xFFE1)
      {}

      /// \brief Constructor with explicit values.
      ExifDecodeOptions(usize max_metadata_size, usize header_offset, u16 metadata_marker)
        : max_metadata_size(max_metadata_size)
        , header_offset(header_offset)
        , metadata_marker(metadata_marker)
      {}

      usize max_metadata_size; ///< Maximum metadata size in bytes. Defaults to <code>1024</code>.
      usize header_offset;     ///< Header offset in bytes. Defaults to <code>20</code>.
      u16 metadata_marker;     ///< Metadata marker. Defaults to <code>0xFFE1</code>.
    };

    /// \brief Image type enumeration.
    enum class ImageType : char
    {
      Telescopic = 0, ///< Telescopic image (triangular).
      Strip = 1       ///< Strip image (rectangular).
    };

    /// \brief SAR mode enumeration.
    enum class Mode : char
    {
      Unknown = 0, ///< Unknown SAR mode.
      M1 = 1,      ///< M1 SAR mode.
      M2 = 2,      ///< M2 SAR mode.
      M3 = 3,      ///< M3 SAR mode.
      M4 = 4,      ///< M4 SAR mode.
      M5 = 5,      ///< M5 SAR mode.
      M6 = 6,      ///< M6 SAR mode.
      M7 = 7,      ///< M7 SAR mode.
    };

    /// \brief WGS84 coordinate placeholder.
    struct Coordinate
    {
      /// \brief Latitude in WGS84 datum of image anchor point (°).
      f64 latitude;

      /// \brief Longitude in WGS84 datum of image anchor point (°).
      f64 longitude;

      /// \brief Altitude in the moment of capture rel. to sea level (m).
      f32 altitude;
      // maybe e0?
    };

    virtual ~Metadata() = default;

    /// \brief Converts metadata object to string.
    /// \details Example string representation:
    /// \code {.js}
    /// 'm1-13-12-2022_20-51-15': {
    ///   anchor: [55.482986°, 36.710065°, 66.240005],
    ///   resolution: [1, 1] m/px,
    ///   near edge: 100.000 m,
    ///   frame offset: -200.000,
    ///   azimuth: 335.06°N,
    ///   drift angle: -0.32°,
    ///   size: [2000, 400] m,
    ///   arc divergence: 20.00°,
    ///   velocity: 30.214148 m/s,
    ///   fic: 1,
    ///   time shift: 1 s,
    ///   time duration: 1 s,
    ///   sar mode: Unknown,
    ///   image type: Telescopic,
    ///   valid: false
    /// }
    /// \endcode
    /// \returns String representation of the metadata.
    [[nodiscard]] virtual auto to_string() const -> std::string override;

    /// \brief Image name.
    /// \details Usually the name of the image file without extension.
    [[nodiscard]] auto name() const -> string_view {
      return this->image_name_;
    }

    /// \brief Image anchor point coordinates in WGS84 datum (°).
    /// \details Corresponds to <tt>latitude</tt>, <tt>longitude</tt> and <tt>altitude</tt> fields in former
    /// versions of QuaSAR/Corona.
    [[nodiscard]] auto anchor_point() const -> Coordinate const& {
      return this->anchor_point_;
    }

    /// \brief Image resolution in m/pixel.
    [[nodiscard]] auto resolution() const -> fl::math::size2d<f32> const& {
      return this->resolution_;
    }

    /// \brief Distance to the near edge of the image in meters.
    /// \details Equivalent to <tt>x0</tt> field in former versions of QuaSAR/Corona.
    [[nodiscard]] auto near_edge() const -> fl::math::length<f32> const& {
      return this->near_edge_;
    }

    /// \brief Frame offset in meters.
    /// \details Equivalent to <tt>y0</tt> field in former versions of QuaSAR/Corona.
    /// \note Currently unused.
    [[nodiscard]] auto frame_offset() const -> fl::math::length<f32> const& {
      return this->frame_offset_;
    }

    /// \brief Azimuth of the image in degrees from north.
    /// \details Equivalent to <tt>azimuth</tt> or <tt>angle</tt> field in former versions of QuaSAR/Corona.
    [[nodiscard]] auto azimuth() const -> fl::math::angle<f32> const& {
      return this->azimuth_;
    }

    /// \brief Drift angle in degrees.
    [[nodiscard]] auto drift_angle() const -> fl::math::angle<f32> const& {
      return this->drift_angle_;
    }

    /// \brief Image width and height in meters.
    /// \details Corresponds to <tt>lx</tt> and <tt>ly</tt> fields in former versions of QuaSAR/Corona.
    [[nodiscard]] auto size() const -> fl::math::size2d<f32> const& {
      return this->size_;
    }

    /// \brief Arc divergence in degrees.
    /// \details Corresponds to <tt>theta_azimuth</tt> or <tt>div</tt> field in former versions of QuaSAR/Corona.
    [[nodiscard]] auto arc_divergence() const -> fl::math::angle<f32> const& {
      return this->arc_divergence_;
    }

    /// \brief Velocity in m/s in the moment of capture.
    [[nodiscard]] auto velocity() const -> f32 {
      return this->velocity_;
    }

    /// \brief Frequency interpolation coefficient (FIC).
    /// \details Corresponds to <tt>kR</tt> field in former versions of QuaSAR/Corona.
    [[nodiscard]] auto frequency_interpolation_coefficient() const -> f32 {
      return this->frequency_interpolation_coefficient_;
    }

    /// \brief Image offset from time of capture (s)
    /// \details Corresponds to <tt>TShift</tt> field in former versions of QuaSAR/Corona.
    [[nodiscard]] auto time_shift() const -> f32 {
      return this->time_shift_;
    }

    /// \brief Capture duration (s)
    /// \details Corresponds to <tt>Ts</tt> field in former versions of QuaSAR/Corona.
    [[nodiscard]] auto time_duration() const -> f32 {
      return this->time_duration_;
    }

    /// \brief SAR mode.
    /// \details Corresponds to <tt>mode</tt> field in former versions of QuaSAR/Corona.
    [[nodiscard]] auto sar_mode() const -> Mode {
      return this->sar_mode_;
    }

    /// \brief Image type.
    /// \details Corresponds to <tt>type</tt> field in former versions of QuaSAR/Corona.
    [[nodiscard]] auto image_type() const -> ImageType {
      return this->image_type_;
    }

    /// \brief Returns <tt>true</tt> if checksum is valid.
    [[nodiscard]] auto valid() const -> bool {
      return this->crc_valid_;
    }

    /// \brief Parses and constructs new Metadata object from given path to image file.
    /// \param image_file Path to image file.
    /// \param options Optional decoder options. Default: default options.
    /// \returns Metadata object with parsed data.
    /// \throws corona::image::decode_error if parsing failed.
    [[nodiscard]] static auto from_exif_file(
      fs::path const& image_file,
      ExifDecodeOptions const& options = ExifDecodeOptions()
    ) noexcept(false) -> Metadata;

    /// \brief Parses and constructs new Metadata object from given view over EXIF data.
    /// \param exif_data View over EXIF data. Must be at least the size of this object plus all required offsets.
    /// \param relative Whether the offsets in <tt>exif_data</tt> are relative to the start of the view. If this flag is
    /// set, parser will assume that first byte in <tt>exif_data</tt> is the start of needed data. Otherwise, constant
    /// <tt>JPEG_HEADER_SIZE</tt> will be used as the start of needed data. Default: <tt>false</tt>.
    /// \param options Optional decoder options. Default: default options.
    /// \returns Metadata object with parsed data.
    /// \throws corona::image::decode_error if parsing failed.
    [[nodiscard]] static auto from_exif_data(
      option<string_view> filename,
      span<byte const> exif_data,
      bool relative = false,
      ExifDecodeOptions const& options = ExifDecodeOptions()
    ) noexcept(false) -> Metadata;

    /// \brief Returns <tt>true</tt> if checksum of metadata is valid.
    [[nodiscard]] explicit operator bool() const { return this->valid(); }

    private:
     string image_name_;
     Coordinate anchor_point_;
     fl::math::size2d<f32> resolution_;
     fl::math::length<f32> near_edge_;
     [[maybe_unused]] fl::math::length<f32> frame_offset_;
     fl::math::angle<f32> azimuth_;
     fl::math::angle<f32> drift_angle_;
     fl::math::size2d<f32> size_;
     fl::math::angle<f32> arc_divergence_;
     f32 velocity_;
     f32 frequency_interpolation_coefficient_;
     f32 time_shift_;
     f32 time_duration_;
     Mode sar_mode_;
     ImageType image_type_;
     bool crc_valid_;
  };
} // namespace corona::image
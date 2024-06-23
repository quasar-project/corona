#pragma once

#include <corona/detail/export.h>
#include <floppy/euclid.h>

namespace corona::image::metadata
{
  struct Metadata
  {
    /// \brief Image type enumeration.
    enum class ImageType
    {
      Telescopic = 0, ///< Telescopic image (triangular).
      Strip = 1       ///< Strip image (rectangular).
    };

    /// \brief SAR mode enumeration.
    enum class Mode
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
    // coordinate with altitude Latitude in WGS84 datum of image anchor point (°) Altitude in the moment of capture rel. to sea level (m)

    /// \brief Image resolution in m/pixel.
    fl::math::size2d<f32> resolution;

    /// \brief Distance to the near edge of the image in meters.
    /// \details Equivalent to <tt>x0</tt> field in former versions of QuaSAR/Corona.
    fl::math::length<f32> near_edge;

    /// \brief Frame offset in meters.
    /// \details Equivalent to <tt>y0</tt> field in former versions of QuaSAR/Corona.
    /// \note Currently unused.
    [[maybe_unused]] fl::math::length<f32> frame_offset;

    /// \brief Azimuth of the image in degrees from north.
    /// \details Equivalent to <tt>azimuth</tt> or <tt>angle</tt> field in former versions of QuaSAR/Corona.
    fl::math::angle<f32> azimuth;

    /// \brief Drift angle in degrees.
    fl::math::angle<f32> drift_angle;

    /// \brief Image width and height in meters.
    /// \details Corresponds to <tt>lx</tt> and <tt>ly</tt> fields in former versions of QuaSAR/Corona.
    fl::math::size2d<f32> size;

    /// \brief Arc divergence in degrees.
    /// \details Corresponds to <tt>theta_azimuth</tt> or <tt>div</tt> field in former versions of QuaSAR/Corona.
    fl::math::angle<f32> arc_divergence;

    /// \brief Velocity in m/s in the moment of capture.
    f32 velocity;

    /// \brief Frequency interpolation coefficient (FIC).
    /// \details Corresponds to <tt>kR</tt> field in former versions of QuaSAR/Corona.
    f32 frequency_interpolation_coefficient;

    /// \brief Image offset from time of capture (s)
    /// \details Corresponds to <tt>TShift</tt> field in former versions of QuaSAR/Corona.
    f32 time_shift;

    /// \brief Capture duration (s)
    /// \details Corresponds to <tt>Ts</tt> field in former versions of QuaSAR/Corona.
    f32 time_duration;

    /// \brief SAR mode.
    /// \details Corresponds to <tt>mode</tt> field in former versions of QuaSAR/Corona.
    Mode sar_mode;

    /// \brief Image type.
    /// \details Corresponds to <tt>type</tt> field in former versions of QuaSAR/Corona.
    ImageType image_type;

    private:
      bool crc_valid_;
  };
} // namespace corona::image::metadata
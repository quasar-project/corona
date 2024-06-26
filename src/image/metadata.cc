#include <bit>
#include <fstream>
#include <floppy/logging.h>
#include <corona/image/metadata.h>

namespace
{
  using namespace fl::types;

  /// \brief JPEG metadata header. Serialized in big endian.
  struct ExifHeader
  {
    u16 marker;
    u16 size;
  };
} // namespace

namespace corona::image
{
  auto Metadata::from_exif_file(
    fs::path const& image_file,
    ExifDecodeOptions const& options
  ) noexcept(false) -> Metadata {
    auto ifs = std::ifstream(image_file, std::ios::binary);
    if(not ifs)
      throw decoding_error(image_file, "failed to open file");
    // read first 1024 bytes
    auto exif_data = std::vector<byte>(options.max_metadata_size);
    ifs.read(reinterpret_cast<char*>(exif_data.data()), static_cast<std::streamsize>(exif_data.size())); // NOLINT(*-pro-type-reinterpret-cast)
    if(static_cast<usize>(ifs.gcount()) < exif_data.size())
      throw decoding_error(image_file, fmt::format("exif data too small: {} bytes of at least {} bytes",
        ifs.gcount(),
        exif_data.size())
      );
    return from_exif_data(image_file.filename().string(), exif_data, true, options);
  }

  auto Metadata::from_exif_data(
    option<string_view> filename,
    span<byte const> exif_data,
    bool const relative,
    ExifDecodeOptions const& options
  ) noexcept(false) -> Metadata {
    if(relative)
      exif_data = exif_data.subspan(options.header_offset);
    if(exif_data.size() < sizeof(ExifHeader))
      throw decoding_error("exif data too small");
    auto header = ExifHeader();
    header = *reinterpret_cast<ExifHeader const*>(exif_data.data()); // NOLINT(*-pro-type-reinterpret-cast)
    if constexpr(std::endian::native != std::endian::big)
      header = fl::platform::swap_endian(header);
    if(header.marker != options.metadata_marker)
      throw decoding_error(filename.value_or("raw data"), fmt::format("invalid exif marker: 0x{:04X}, expected 0x{:04X}",
        header.marker,
        options.metadata_marker
      ));
    exif_data = exif_data.subspan(sizeof(ExifHeader));
    if(exif_data.size() < header.size)
      throw decoding_error(filename.value_or("raw data"), fmt::format("exif data too small: {} bytes of at least {} bytes",
        exif_data.size(),
        header.size
      ));
    auto self = Metadata();
    self.image_name_ = filename.value_or("raw data");
    // NOLINTBEGIN(*-pro-type-reinterpret-cast)
    self.anchor_point_ = {
      .latitude = *reinterpret_cast<f64 const*>(exif_data.data()),
      .longitude = *reinterpret_cast<f64 const*>(exif_data.data() + sizeof(f64)),
      .altitude = *reinterpret_cast<f32 const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 10)
    };
    self.anchor_point_.latitude = *reinterpret_cast<f64 const*>(exif_data.data());
    self.anchor_point_.longitude = *reinterpret_cast<f64 const*>(exif_data.data() + sizeof(f64));
    self.resolution_ = {
      *reinterpret_cast<f32 const*>(exif_data.data() + sizeof(f64) * 2),
      *reinterpret_cast<f32 const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32))
    };
    self.near_edge_ = *reinterpret_cast<f32 const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 2);
    self.frame_offset_ = *reinterpret_cast<f32 const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 3);
    self.azimuth_ = fl::math::angle<f32>::from_radians(*reinterpret_cast<f32 const*>(
      exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 4)
    );
    self.drift_angle_ = fl::math::angle<f32>::from_radians(*reinterpret_cast<f32 const*>(
      exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 5)
    );
    self.size_ = {
      *reinterpret_cast<f32 const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 6),
      *reinterpret_cast<f32 const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 7)
    };
    self.arc_divergence_ = fl::math::angle<f32>::from_radians(*reinterpret_cast<f32 const*>(
      exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 8)
    );
    self.velocity_ = *reinterpret_cast<f32 const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 9);
    self.frequency_interpolation_coefficient_ = *reinterpret_cast<f32 const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 11);
    self.time_shift_ = *reinterpret_cast<f32 const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 12);
    self.time_duration_ = *reinterpret_cast<f32 const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 13);
    self.sar_mode_ = *reinterpret_cast<Mode const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 16);
    self.image_type_ = *reinterpret_cast<ImageType const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 16 + sizeof(char));
    //auto const crc = *reinterpret_cast<u16 const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 18 + sizeof(char) * 2);
    //self.crc_valid_ =
    // todo: crc
    // NOLINTEND(*-pro-type-reinterpret-cast)
    //fl::log::debug("decoded metadata for {}", self.name());
    // todo: fix loggers
    // https://github.com/quasar-project/deko/blob/main/src/decoder/jpeg_decoder.rs
    // http://uav.radar-mms.com/gitlab/developers/rls/quasar/-/blob/qt5/src/c++/processing/imageprocessing.c++?ref_type=heads
    return self;
  }
} // namespace corona::image
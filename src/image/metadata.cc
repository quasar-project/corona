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
    std::ignore = image_file;
    // todo
    throw std::runtime_error("not implemented");
  }

  auto Metadata::from_exif_data(
    span<byte const> exif_data,
    bool const relative,
    ExifDecodeOptions const& options
  ) noexcept(false) -> Metadata {
    if(relative)
      exif_data = exif_data.subspan(options.header_offset);

    // https://github.com/quasar-project/deko/blob/main/src/decoder/jpeg_decoder.rs
    // http://uav.radar-mms.com/gitlab/developers/rls/quasar/-/blob/qt5/src/c++/processing/imageprocessing.c++?ref_type=heads
    std::ignore = exif_data;
    std::ignore = relative;
    // todo
    throw std::runtime_error("not implemented");
  }
} // namespace corona::image
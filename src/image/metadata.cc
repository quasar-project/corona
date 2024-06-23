#include <corona/image/metadata.h>

namespace corona::image
{
  Metadata::Metadata(fs::path const& image_file) noexcept(false) {
    // todo
    throw std::runtime_error("not implemented");
  }

  Metadata::Metadata(
    span<const u8> const exif_data,
    bool const relative
  ) noexcept(false) {
    // todo
    throw std::runtime_error("not implemented");
  }

  auto Metadata::from_exif_file(fs::path const& image_file) noexcept(false) -> Metadata {
    // todo
    throw std::runtime_error("not implemented");
  }

  auto Metadata::from_exif_data(
    span<const u8> const exif_data,
    bool const relative
  ) noexcept(false) -> Metadata {
    // todo
    throw std::runtime_error("not implemented");
  }
} // namespace corona::image
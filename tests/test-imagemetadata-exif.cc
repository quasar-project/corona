#include <gtest/gtest.h>
#include <corona/image/metadata/exif_metadata.h>

using namespace std::literals;

namespace {
  static auto const test_images = std::vector<std::filesystem::path> {
    std::filesystem::current_path() / "data" / "m1-13-12-2022_20-51-15.jpg",
    std::filesystem::current_path() / "data" / "m1-13-12-2022_20-53-26.jpg",
    std::filesystem::current_path() / "data" / "m3-13-12-2022_21-26-14.jpg"
  };
} // namespace

TEST(ImageMetadataExif, Prelude)
{
  for(auto const path : ::test_images)
    EXPECT_TRUE(exists(path));
}
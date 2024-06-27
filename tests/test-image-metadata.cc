#include <corona/image/metadata.h>

#include <gtest/gtest.h>
#include "include.h"

using namespace std::literals;

namespace {
  auto const test_images = std::vector<std::filesystem::path> {
    tests::data("m1-13-12-2022_20-51-15.jpg"),
    tests::data("m1-13-12-2022_20-53-26.jpg"),
    tests::data("m3-13-12-2022_21-26-14.jpg")
  };
} // namespace

TEST(ImageMetadata, Prelude)
{
  for(auto const& path : ::test_images)
    EXPECT_TRUE(exists(path));
}

TEST(ImageMetadata, FromExifFile)
{
  auto res = corona::image::Metadata::from_exif_file(::test_images[0]);
}
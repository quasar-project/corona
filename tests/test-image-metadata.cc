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
  fmt::print("{}\n", res);
  EXPECT_EQ(res.name(), "m1-13-12-2022_20-51-15");
  EXPECT_FLOAT_EQ(res.anchor_point().latitude, 55.482986);
  EXPECT_FLOAT_EQ(res.anchor_point().longitude, 36.710065);
  EXPECT_FLOAT_EQ(res.anchor_point().altitude, 66.240005);
  auto const ref_resolution = fl::math::size2d<float>(1.0F, 1.0F);
  EXPECT_EQ(res.resolution(), ref_resolution);
  EXPECT_EQ(res.near_edge(), 100.0F);
  EXPECT_EQ(res.frame_offset(), -200.0F);
  EXPECT_FLOAT_EQ(res.azimuth().degrees(), 335.06F);
  EXPECT_FLOAT_EQ(res.drift_angle().degrees(), -0.32262033F);
  EXPECT_FLOAT_EQ(res.size().width(), 2000.0F);
  EXPECT_FLOAT_EQ(res.size().height(), 400.0F);
  EXPECT_FLOAT_EQ(res.arc_divergence().degrees(), 20.0F);
  EXPECT_FLOAT_EQ(res.velocity(), 30.214148F);
  EXPECT_FLOAT_EQ(res.frequency_interpolation_coefficient(), 1.0F);
  EXPECT_FLOAT_EQ(res.time_shift(), 1.0F);
  EXPECT_FLOAT_EQ(res.time_duration(), 1.0F);
  EXPECT_EQ(res.sar_mode(), corona::image::Metadata::Mode::Unknown);
  EXPECT_EQ(res.image_type(), corona::image::Metadata::ImageType::Telescopic);
  EXPECT_FALSE(res);
}
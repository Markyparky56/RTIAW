#include <vector>
#include <chrono>
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "glm-format.hpp"
#include "fmt/chrono.h"
#include "entt/entity/registry.hpp"
#include "common.hpp"
#include "pixel.hpp"
#include "ray.hpp"

int main()
{
  const int32 imageWidth = 1024;
  const int32 imageHeight = 1024;

  std::vector<Pixel3> imgBuffer;
  imgBuffer.reserve(imageWidth * imageHeight);

  const int32 maxY = imageHeight - 1;
  const int32 maxX = imageWidth - 1;
  const int32 progressUpdate = imageHeight / 10;
  for (int32 y = maxY; y >= 0; --y)
  {
    for (int32 x = 0; x < imageWidth; ++x)
    {
      constexpr real m = 256 - KindaSmallEpsilon;
      const colour col = { real(x) / maxX, real(y) / maxY, real(0.25) };
      imgBuffer.emplace_back(uint8(m * col.r), uint8(m * col.g), uint8(m * col.b));
    }

    // Check progress
    const int32 step = (imageHeight - y);
    const real normalised = real(step) / real(imageHeight);
    const real percent = normalised * real(100);
    if (percent > 0 && step % progressUpdate == 0)
    {
      fmt::print("{}% done...\n", percent);
    }
  }

  auto timeNow = std::chrono::system_clock::now();
  std::string filename = fmt::format("rtiaw-{:%d-%H-%M-%S}.png", timeNow);

  fmt::print("Done! Writing to file {} ...", filename);

  if (stbi_write_png(filename.c_str(), imageWidth, imageHeight, 3, imgBuffer.data(), 3 * imageWidth))
  {
    fmt::print("File written!", filename);
  }
  else
  {
    fmt::print("Failed to output file!");
  }

  return 0;
}

#include <vector>
#include <chrono>
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "glm-format.hpp"
#include "common.hpp"
#include "fmt/chrono.h"

struct Pixel3
{
  Pixel3(uint8 _r, uint8 _g, uint8 _b)
    : r(_r)
    , g(_g)
    , b(_b)
  {}
  uint8 r, g, b;
};

int main()
{
  const int32 imageWidth = 256;
  const int32 imageHeight = 256;

  std::vector<Pixel3> imgBuffer;
  imgBuffer.reserve(imageWidth * imageHeight);

  const int32 maxY = imageHeight - 1;
  const int32 maxX = imageWidth - 1;
  for (int32 y = maxY; y >= 0; --y)
  {
    for (int32 x = 0; x < imageWidth; ++x)
    {
      real r = real(x) / maxX;
      real g = real(y) / maxY;
      real b = real(0.25);

      constexpr real m = 256 - KindaSmallEpsilon;
      imgBuffer.emplace(end(imgBuffer), uint8(m * r), uint8(m * g), uint8(m * b));
    }
  }

  auto timeNow = std::chrono::system_clock::now();
  std::string filename = fmt::format("rtiaw-{:%d-%H-%M-%S}.png", timeNow);
  stbi_write_png(filename.c_str(), imageWidth, imageHeight, 3, imgBuffer.data(), 3 * imageWidth);

  return 0;
}

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

colour RayColour(const Ray& ray)
{
  const vec3 unitDirection = glm::normalize(ray.GetDirection());
  const real t = real(0.5) * (unitDirection.y + real(1.0));
  return (real(1.0) - t) * colour(1.0) + t * colour(0.5, 0.7, 1.0);
}

int main()
{
  // Image
  constexpr real aspectRatio = real(16) / real(9);
  constexpr int32 imageWidth = 400;
  constexpr int32 imageHeight = int32(imageWidth / aspectRatio);

  std::vector<Pixel3> imgBuffer;
  imgBuffer.reserve(imageWidth * imageHeight);

  // Camera
  constexpr real viewportHeight = 2.0;
  constexpr real viewportWidth = aspectRatio * viewportHeight;
  constexpr real focalLength = 1.0;

  constexpr vec3 origin = vec3(0);
  constexpr vec3 horizontal = vec3(viewportWidth, 0, 0);
  constexpr vec3 vertical = vec3(0, viewportHeight, 0);
  constexpr vec3 lowerLeftCorner = origin - (horizontal / real(2)) - (vertical / real(2)) - vec3(0, 0, focalLength);
    
  // Render
  const int32 maxY = imageHeight - 1;
  const int32 maxX = imageWidth - 1;
  const real invMaxY = real(1) / maxY;
  const real invMaxX = real(1) / maxX;
  const int32 progressUpdate = imageHeight / 10;
  for (int32 y = maxY; y >= 0; --y)
  {
    for (int32 x = 0; x < imageWidth; ++x)
    {
      //const colour col = { real(x) / maxX, real(y) / maxY, real(0.25) };

      const real u = real(x) * invMaxX;
      const real v = real(y) * invMaxY;

      Ray ray(
        origin, 
        lowerLeftCorner + u*horizontal + v*vertical - origin
      );
      const colour col = RayColour(ray);

      imgBuffer.emplace_back(col);
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

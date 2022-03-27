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
#include "hittestsystem.hpp"
#include "entityfactories.hpp"
#include "camera.hpp"

static prng SMTRng;

colour RayColour(const Ray& ray, const entt::registry& scene)
{
  HitTestSystem hitTestSystem;
  HitResult result;

  if (hitTestSystem.TestScene(scene, ray, 0, infinity, result))
  {
    return real(0.5) * colour(result.normal + vec3(1));
  }

  const vec3 unitDirection = glm::normalize(ray.GetDirection());
  const real bg = real(0.5) * (unitDirection.y + real(1.0));
  return (real(1.0) - bg) * colour(1.0) + bg * colour(0.5, 0.7, 1.0);
}

void ReduceMultiSampledColour(colour& inCol, const real invSamplesPerPixel)
{
  inCol *= invSamplesPerPixel;
}

int main()
{
  SeedRNG();

  // Image
  constexpr real aspectRatio = real(16) / real(9);
  constexpr int32 imageWidth = 400;
  constexpr int32 imageHeight = int32(imageWidth / aspectRatio);

  std::vector<Pixel3> imgBuffer;
  imgBuffer.reserve(imageWidth * imageHeight);

  // Camera
  Camera camera;
  constexpr uint32 samplesPerPixel = 100;
  constexpr real invSamplesPerPixel = real(1.0) / real(samplesPerPixel);

  // Registry
  entt::registry sceneRegistry;
  CreateSphere(sceneRegistry, vec3(0, 0, -1), 0.5);
  CreateSphere(sceneRegistry, vec3(0, -100.5, -1), 100);
   
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
      colour col = colour(0);
      for (int32 sample = 0; sample < samplesPerPixel; ++sample)
      {
        vec2 uv(
          (real(x) + GetRandomReal()) * invMaxX,
          (real(y) + GetRandomReal()) * invMaxY
        );
        Ray ray = camera.GetRay(uv);
        col += RayColour(ray, sceneRegistry);
      }
      ReduceMultiSampledColour(col, invSamplesPerPixel);     

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

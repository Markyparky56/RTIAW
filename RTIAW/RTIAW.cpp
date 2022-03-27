#include <vector>
#include <chrono>
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "glm-format.hpp"
#include "fmt/chrono.h"
#include "entt/entity/registry.hpp"
#include "common.hpp"
#include "utility.hpp"
#include "pixel.hpp"
#include "ray.hpp"
#include "hittestsystem.hpp"
#include "entityfactories.hpp"
#include "camera.hpp"
#include "MaterialFunctions.hpp"

prng GMTRng;

colour RayColour(const Ray& ray, const entt::registry& scene, int32 depth)
{
  // Safety check to avoid stack overflows from infinite bounces
  if (depth <= 0)
  {
    return colour(0);
  }

  static HitTestSystem hitTestSystem;
  HitResult result;

  if (hitTestSystem.TestScene(scene, ray, KindaSmallEpsilon, infinity, result))
  {
    if(scene.valid(result.entity))
    {
      Ray scatteredRay;
      colour attenuation;
      
      // Get material function from hit entity
      const material& mat = scene.get<material>(result.entity);
      if (mat.materialFunc && (*mat.materialFunc)(scene, ray, result, attenuation, scatteredRay))
      {
        return attenuation * RayColour(scatteredRay, scene, --depth);
      }
      else
      {
        return colour(0);
      }
    }

    // Calculate bounce direction
    const vec3 target = result.pos + result.normal + GetRandomUnitVector();

    return real(0.5) * RayColour(Ray(result.pos, target - result.pos), scene, --depth);
  }

  const vec3 unitDirection = glm::normalize(ray.GetDirection());
  const real bg = real(0.5) * (unitDirection.y + real(1.0));
  return (real(1.0) - bg) * colour(1.0) + bg * colour(0.5, 0.7, 1.0);
}

void ReduceMultiSampledColour(colour& inCol, const real invSamplesPerPixel, const bool gammeCorrect=true)
{
  inCol *= invSamplesPerPixel;
  if (gammeCorrect)
  {
    inCol = glm::sqrt(inCol);
  }
}

void CreateRandomScene(entt::registry& registry)
{
  const LambertianData ground{ colour(real(0.5)) };
  CreateSphere(registry, position(0, -1000, 0), 1000, &LambertianMaterial, ground);

  for (int32 a = -11; a < 11; ++a)
  {
    for (int32 b = -11; b < 11; ++b)
    {
      const real rndMat = GetRandomReal();
      const position centre(a + real(0.9) * GetRandomReal(), 0.2, b + real(0.9) * GetRandomReal());

      if (glm::length(centre - position(4, 0.2, 0)) > 0.9)
      {
        if (rndMat < real(0.8))
        {
          // Diffuse
          const colour albedo = GetRandomVec3() * GetRandomVec3();
          const LambertianData lamData{ albedo };
          CreateSphere(registry, centre, 0.2, &LambertianMaterial, lamData);
        }
        else if (rndMat < real(0.95))
        {
          // Metal
          const colour albedo = GetRandomVec3(real(0.5), real(1.0));
          const real fuzz = GetRandomReal();
          MetalData metData{ albedo, fuzz };
          CreateSphere(registry, centre, 0.2, &MetalMaterial, metData);
        }
        else
        {
          // Glass
          const DielectricData diData{ real(1.5) };
          CreateSphere(registry, centre, 0.2, &DielectricMaterial, diData);
        }
      }
    }
  }

  const DielectricData diData15{ real(1.5) };
  CreateSphere(registry, position(0, 1, 0), real(1.0), &DielectricMaterial, diData15);

  const LambertianData lamData{ colour(real(0.4), real(0.2), real(0.1)) };
  CreateSphere(registry, position(-4, 1, 0), 1.0, &LambertianMaterial, lamData);
  
  const MetalData metData{ colour(real(0.7), real(0.6), real(0.5)), 0.0 };
  CreateSphere(registry, position(4, 1, 0), 1.0, &MetalMaterial, metData);
}

int main()
{
  SeedRNG();

  // Image
  constexpr real aspectRatio = real(3) / real(2); // real(16) / real(9);
  constexpr int32 imageWidth = 1200; // 400;
  constexpr int32 imageHeight = int32(imageWidth / aspectRatio);
  constexpr uint32 samplesPerPixel = 50;// 100;
  constexpr real invSamplesPerPixel = real(1.0) / real(samplesPerPixel);
  constexpr int32 maxBounces = 50;

  std::vector<Pixel3> imgBuffer;
  imgBuffer.reserve(imageWidth * imageHeight);

  // Camera
  position lookFrom(13, 2, 3);
  position lookAt(0, 0, 0);
  vec3 vup(0, 1, 0);
  real distToFocus = 10;// glm::length(lookFrom - lookAt);
  real aperture = 0.1;
  Camera camera(20, aspectRatio, aperture, distToFocus, lookFrom, lookAt, vup);

  const real R = glm::cos(pi / real(4));

  // Material data
  LambertianData red{ colour(real(1.0), 0, 0) };
  LambertianData trueBlue{ colour(0, 0, real(1.0)) };
  LambertianData yellow{ colour(real(0.8), real(0.8), real(0.0)) };
  LambertianData orange{ colour(real(0.7), real(0.3), real(0.3)) };
  LambertianData blue{ colour(real(0.1), real(0.2), real(0.5)) };
  MetalData lightGrey{ colour(real(0.8)), real(0.3)};
  MetalData bronze{ colour(real(0.8), real(0.6), real(0.2)), real(1.0) };
  DielectricData glass15{ real(1.5) };

  // Registry
  entt::registry sceneRegistry;
  CreateRandomScene(sceneRegistry);

  //CreateSphere(sceneRegistry, vec3(0, -100.5, -1), 100, &LambertianMaterial, yellow); // ground
  //CreateSphere(sceneRegistry, vec3(0, 0, -1), real(0.5), &LambertianMaterial, blue); // centre
  //CreateSphere(sceneRegistry, vec3(-1, 0, -1), real(0.5), &DielectricMaterial, glass15); // left
  //CreateSphere(sceneRegistry, vec3(-1, 0, -1), real(-0.4), &DielectricMaterial, glass15); // left
  //CreateSphere(sceneRegistry, vec3(1, 0, -1), real(0.5), &MetalMaterial, bronze); // right

  //CreateSphere(sceneRegistry, vec3(-R, 0, -1), R, &LambertianMaterial, trueBlue);
  //CreateSphere(sceneRegistry, vec3(R, 0, -1), R, &LambertianMaterial, red);
   
  // Render
  const int32 maxY = imageHeight - 1;
  const int32 maxX = imageWidth - 1;
  const real invMaxY = real(1) / maxY;
  const real invMaxX = real(1) / maxX;
  const int32 progressUpdate = imageHeight / 100;
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
        col += RayColour(ray, sceneRegistry, maxBounces);
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

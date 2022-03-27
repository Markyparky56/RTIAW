#include "camera.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"

Camera::Camera(const real vFov, const real aspectRatio, const real focalLength, const position& pos, const position& lookAt, const vec3& vup)
{
  const real theta = glm::radians(vFov);
  const real h = glm::tan(theta/real(2));
  const real viewportHeight = real(2) * h;
  const real viewportWidth = aspectRatio * viewportHeight;

  const vec3 w = glm::normalize(pos - lookAt);
  const vec3 u = glm::normalize(glm::cross(vup, w));
  const vec3 v = glm::cross(w, u);

  Pos = pos;
  horizontal = viewportWidth * u;
  vertical = viewportHeight * v;
  LowerLeftCorner = Pos - (horizontal / real(2)) - (vertical / real(2)) - w;
}

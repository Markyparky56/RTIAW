#include "camera.hpp"
#include "utility.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"

Camera::Camera(const real vFov, const real aspectRatio, const real aperture, const real focusDistance, const position& pos, const position& lookAt, const vec3& vup)
{
  const real theta = glm::radians(vFov);
  const real h = glm::tan(theta/real(2));
  const real viewportHeight = real(2) * h;
  const real viewportWidth = aspectRatio * viewportHeight;

  w = glm::normalize(pos - lookAt);
  u = glm::normalize(glm::cross(vup, w));
  v = glm::cross(w, u);

  Pos = pos;
  horizontal = focusDistance * viewportWidth * u;
  vertical = focusDistance * viewportHeight * v;
  LowerLeftCorner = Pos - (horizontal / real(2)) - (vertical / real(2)) - focusDistance*w;

  LensRadius = aperture / 2;
}

Ray Camera::GetRay(vec2 uv) const
{
  const vec3 rd = LensRadius * GetRandomInUnitDisc();
  const vec3 offset = u * rd.x + v * rd.y;

  return Ray(
    Pos + offset, 
    LowerLeftCorner + (uv.s * horizontal) + (uv.t * vertical) - Pos - offset
  );
}

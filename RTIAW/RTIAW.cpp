#include "glm-format.hpp"

int main()
{
  fmt::print("Hello, World!\n");

  glm::vec3 a(1.f, -1.f, 2.5f);
  glm::vec3 b(-9.f, 2.f, 42.f);
  glm::vec3 c = a + b;

  fmt::print("{} + {} = {}", a, b, c);

  return 0;
}

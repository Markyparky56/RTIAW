#pragma once
#include <fmt/format.h>
#include <glm/glm.hpp>

/*
* glm::vec3 formatter
*/
template<>
class fmt::formatter<glm::vec3>
{
public:
  // Parse format specification and store it
  constexpr auto parse(format_parse_context& ctx)
  {
    auto i = ctx.begin(), end = ctx.end();
    if (i != end && (*i == 'f' || *i == 'e'))
    {
      spec = *i++;
    }
    if (i != end && *i != '}')
    {
      throw format_error("invalid format");
    }
    return i;
  }

  // Format a value using stored spec
  template<typename FormatContext>
  constexpr auto format(const glm::vec3& v, FormatContext& ctx) const
  {
    switch (spec)
    {
    default:
      // Falls-through
    case 'f': return format_to(ctx.out(), "({:f}, {:f}, {:f})", v.x, v.y, v.z);
    case 'e': return format_to(ctx.out(), "({:e}, {:e}, {:e})", v.x, v.y, v.z);
    }
  }

  char spec = 'f';
};
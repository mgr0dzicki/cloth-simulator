#include "VertexType.hpp"

VertexType VertexType::operator+(VertexType const& other) const {
  VertexType result;
  result.position = position + other.position;
  result.color = color + other.color;
  return result;
}

VertexType VertexType::operator*(float const& f) const {
  VertexType result;
  result.position = position * f;
  result.color = color * f;
  return result;
}

VertexType operator*(float const& f, VertexType const& v) {
  return v * f;
}

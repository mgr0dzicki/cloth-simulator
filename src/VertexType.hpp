#ifndef OPENGL_CMAKE_SKELETON_VERTEX_TYPE_HPP
#define OPENGL_CMAKE_SKELETON_VERTEX_TYPE_HPP

#include <glm/glm.hpp>

class VertexType {
public:
  glm::vec3 position;
  glm::vec3 color;

  VertexType operator+(VertexType const& other) const;
  VertexType operator*(float const& f) const;
};

VertexType operator*(float const& f, VertexType const& v);

#endif  // OPENGL_CMAKE_SKELETON_VERTEX_TYPE_HPP

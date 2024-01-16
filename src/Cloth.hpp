#ifndef OPENGL_CMAKE_SKELETON_CLOTH_HPP
#define OPENGL_CMAKE_SKELETON_CLOTH_HPP

#include <glm/glm.hpp>
#include <vector>
#include "Shader.hpp"

class Node {
 public:
  Node() = default;
  Node(glm::vec3 position, glm::vec3 velocity = glm::vec3(0));
  glm::vec3 getPosition();
  void update(float dt, float prevDt);
  void constrainBall(glm::vec3 center, float radius);

 private:
  glm::vec3 position;
  glm::vec3 prevPosition;
};

class Cloth {
 public:
  Cloth(glm::vec3 pos, glm::vec3 dx, glm::vec3 dy, int width, int height, ShaderProgram shaderProgram);
  void update(float dt, float prevDt);
  void draw();

 private:
  std::vector<std::vector<Node>> nodes;
  ShaderProgram shaderProgram;
};

#endif  // OPENGL_CMAKE_SKELETON_CLOTH_HPP

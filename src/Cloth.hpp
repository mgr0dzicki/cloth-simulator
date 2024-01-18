#ifndef OPENGL_CMAKE_SKELETON_CLOTH_HPP
#define OPENGL_CMAKE_SKELETON_CLOTH_HPP

#include <glm/glm.hpp>
#include <vector>
#include "Shader.hpp"

class Node {
 public:
  Node() = default;
  Node(glm::vec3 position, glm::vec3 velocity = glm::vec3(0));
  void update(float dt, float prevDt);
  void constrainBall(glm::vec3 center, float radius);

  glm::vec3 position;

 private:
  glm::vec3 prevPosition;

  static constexpr float AIR_RESISTANCE = 0.01;
};

class Link {
 public:
  Link(Node* a, Node* b);
  void update();

 private:
  Node *a, *b;
  float length;

  static constexpr float STIFFNESS = 0.5;
};

class Cloth {
 public:
  Cloth(glm::vec3 pos,
        glm::vec3 dx,
        glm::vec3 dy,
        int width,
        int height,
        ShaderProgram shaderProgram);
  void update(float dt, float prevDt);
  void draw();

 private:
  std::vector<std::vector<Node>> nodes;
  std::vector<Link> links;
  ShaderProgram shaderProgram;
};

#endif  // OPENGL_CMAKE_SKELETON_CLOTH_HPP

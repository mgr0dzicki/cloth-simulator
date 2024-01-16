#include "Cloth.hpp"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "VertexType.hpp"
#include "asset.hpp"
#include "glError.hpp"

const glm::vec3 G(0, 0, -9.81);


Node::Node(glm::vec3 position, glm::vec3 velocity)
    : position(position), prevPosition(position - velocity) {}

void Node::update(float dt, float prevDt) {
  glm::vec3 tmp = position;
  if (prevDt != 0.)
    position += (position - prevPosition) * dt / prevDt;
  position += G * dt * dt;
  prevPosition = tmp;
}

// quite poor one...
void Node::constrainBall(glm::vec3 center, float radius) {
  glm::vec3 diff = position - center;
  float len = glm::length(diff);
  if (len < radius) {
    position += diff * (radius - len) / len;
    prevPosition = position;
  }
}

Link::Link(Node *a, Node *b) : a(a), b(b), length(glm::length(a->position - b->position)) {}

void Link::update() {
  glm::vec3 diff = a->position - b->position;
  float currLen = glm::length(diff);
  if (currLen > length) {
    float perc = (currLen - length) / currLen / 2.;
    glm::vec3 off = diff * perc;
    a->position -= off;
    b->position += off;
  }
}

Cloth::Cloth(glm::vec3 pos, glm::vec3 dx, glm::vec3 dy, int width, int height, ShaderProgram shaderProgram)
    : shaderProgram(shaderProgram) {
  dx = dx / static_cast<float>(width);
  dy = dy / static_cast<float>(height);
  nodes.resize(height + 1);
  for (int y = 0; y <= height; ++y) {
    nodes[y].resize(width + 1);
    for (int x = 0; x <= width; ++x) {
      glm::vec3 xx = dx * static_cast<float>(x);
      glm::vec3 yy = dy * static_cast<float>(y);
      nodes[y][x] = Node(pos + xx + yy);
    }
  }

  for (int y = 0; y <= height; ++y) {
    for (int x = 0; x <= width; ++x) {
      if (x < width)
        links.push_back(Link(&nodes[y][x], &nodes[y][x + 1]));
      if (y < height)
        links.push_back(Link(&nodes[y][x], &nodes[y + 1][x]));
      if (x < width && y < height) {
        links.push_back(Link(&nodes[y][x], &nodes[y + 1][x + 1]));
        links.push_back(Link(&nodes[y][x + 1], &nodes[y + 1][x]));
      }
      if (x < width - 1)
        links.push_back(Link(&nodes[y][x], &nodes[y][x + 2]));
      if (y < height - 1)
        links.push_back(Link(&nodes[y][x], &nodes[y + 2][x]));
    }
  }
}

void Cloth::update(float dt, float prevDt)  {
  for (auto& line : nodes)
    for (auto& node : line)
      node.update(dt, prevDt);

  for (int t = 0; t < 10; t++) {
    for (auto& link : links)
      link.update();

    for (auto& line : nodes)
      for (auto& node : line)
        node.constrainBall(glm::vec3(0, 0, 2), 2);
  }
}

void Cloth::draw() {
  std::vector<VertexType> vertices;
  std::vector<GLuint> index;

  for (auto& line : nodes) {
    for (auto& node : line) {
      VertexType v;
      v.position = node.position;
      v.color = glm::vec4(1.0, 1.0, 1.0, 1.0);
      vertices.push_back(v);
    }
  }

  for (size_t y = 0; y < nodes.size() - 1; ++y) {
    for (size_t x = 0; x < nodes[y].size() - 1; ++x) {
      index.push_back((x + 0) + nodes[y].size() * (y + 0));
      index.push_back((x + 1) + nodes[y].size() * (y + 0));
      index.push_back((x + 1) + nodes[y].size() * (y + 1));

      index.push_back((x + 1) + nodes[y].size() * (y + 1));
      index.push_back((x + 0) + nodes[y].size() * (y + 1));
      index.push_back((x + 0) + nodes[y].size() * (y + 0));
    }
  }

  GLuint vbo, ibo, vao;

  // vbo
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexType),
               vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // ibo
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint),
               index.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // vao
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // bind vbo
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // map vbo to shader attributes
  shaderProgram.setAttribute("in_Position", 4, sizeof(VertexType),
                             offsetof(VertexType, position));
  shaderProgram.setAttribute("in_Colour", 3, sizeof(VertexType),
                             offsetof(VertexType, color));

  // bind the ibo
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

  glCheckError(__FILE__, __LINE__);
  glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, NULL);

  glBindVertexArray(0);
}

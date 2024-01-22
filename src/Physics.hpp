#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "Renderer.hpp"
#include "Shader.hpp"
#include <glm/glm.hpp>
#include <vector>

class Cube {
  public:
    Cube(glm::vec3 center, float a, ShaderProgram &shaderProgram);
    void draw();

  private:
    static GLuint vbo, ibo, vao;
    static size_t indexSize;
    glm::mat4 modelMatrix;
    ShaderProgram &shaderProgram;
};

class Node {
  public:
    Node(glm::vec3 position, glm::vec3 velocity = glm::vec3(0.0));
    void update(float dt, float prevDt);
    void constrainBall(glm::vec3 center, float radius);
    void collide(Node &other);

    glm::vec3 position;
    glm::vec3 prevPosition;

  private:
    static constexpr float AIR_RESISTANCE = 0.01;
};

class Link {
  public:
    Link(Node *a, Node *b);
    void update();

  private:
    Node *a, *b;
    float length;

    static constexpr float STIFFNESS = 0.46;
};

class Cloth {
  public:
    Cloth(glm::vec3 pos, glm::vec3 dx, glm::vec3 dy, int width, int height,
          TrianglesShaderProgram &trianglesShaderProgram,
          PointsAndLinesShaderProgram &pointsAndLinesShaderProgram);
    void update(float dt, float prevDt);
    void draw();
    void reset();

    constexpr static int SUBDIVISION_STEPS_MAX = 3;

  private:
    glm::vec3 pos, dx, dy;
    std::vector<std::vector<Node>> nodes;
    std::vector<Link> regularLinks, diagonalLinks, farLinks;
    ClothRenderer clothRenderer;
    MeshRenderer meshRenderer;

    constexpr static int SUBDIVISION_MESH_SIZE_MAX = 241;
};

#endif // PHYSICS_HPP

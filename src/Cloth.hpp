#ifndef CLOTH_HPP
#define CLOTH_HPP

#include "Shader.hpp"
#include <glm/glm.hpp>
#include <vector>

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
    Link(Node *a, Node *b);
    void update();

  private:
    Node *a, *b;
    float length;

    static constexpr float STIFFNESS = 0.46;
};

class MeshDrawer {
  public:
    MeshDrawer(int n, int m, ShaderProgram shaderProgram);
    void draw(std::vector<std::vector<glm::vec3>> const &mesh);

  private:
    std::vector<GLuint> index;
    GLuint vbo, ibo, vao;
    ShaderProgram shaderProgram;
};

class Cloth {
  public:
    Cloth(glm::vec3 pos, glm::vec3 dx, glm::vec3 dy, int width, int height,
          ShaderProgram shaderProgram);
    void update(float dt, float prevDt);
    void draw();
    void setSubdivisionSteps(int subdivisionSteps);

    constexpr static int SUBDIVISION_STEPS_MAX = 3;

  private:
    std::vector<std::vector<Node>> nodes;
    std::vector<Link> regularLinks, diagonalLinks, farLinks;
    MeshDrawer meshDrawer;
    int subdivisionSteps = SUBDIVISION_STEPS_MAX;
    ShaderProgram shaderProgram;

    constexpr static int SUBDIVISION_MESH_SIZE_MAX = 241;
};

#endif // CLOTH_HPP

#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "Renderer.hpp"
#include "Shader.hpp"
#include <glm/glm.hpp>
#include <vector>

class Node {
  public:
    Node(glm::vec3 position, glm::vec3 velocity = glm::vec3(0.0));
    void update(float dt, float prevDt);
    void collide(Node &other, float radius);

    glm::vec3 position;
    glm::vec3 prevPosition;

  private:
    static constexpr float AIR_RESISTANCE = 0.01;
};

class Solid {
  public:
    virtual void draw() = 0;
    virtual void constrain(Node &node) = 0;
};

class Cuboid : public Solid {
  public:
    Cuboid(glm::vec3 a, glm::vec3 b, glm::vec3 colour);
    void draw();
    void constrain(Node &node);
    static void initRenderer(ShaderProgram shaderProgram);

  private:
    static CuboidRenderer *renderer;
    glm::vec3 a, b, colour;
    glm::mat4 modelMatrix;
};

class Ball : public Solid {
  public:
    Ball(glm::vec3 center, float radius, glm::vec3 colour);
    void draw();
    void constrain(Node &node);

  private:
    glm::vec3 center;
    float radius;
    glm::vec3 colour;
    glm::mat4 modelMatrix;
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
    Cloth(glm::vec3 pos, glm::vec3 dx, glm::vec3 dy, glm::vec3 nodesColour,
          glm::vec3 linksColour, glm::vec3 frontColour, glm::vec3 backColour,
          TrianglesShaderProgram &trianglesShaderProgram,
          PointsAndLinesShaderProgram &pointsAndLinesShaderProgram);
    void update(float dt, float prevDt, std::vector<Solid *> const &solid);
    void draw();

  private:
    glm::vec3 pos, dx, dy;
    std::vector<std::vector<Node>> nodes;
    std::vector<Link> regularLinks, diagonalLinks, farLinks;
    ClothRenderer clothRenderer;
    MeshRenderer meshRenderer;
};

#endif // PHYSICS_HPP

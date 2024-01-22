#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Shader.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Renderer {
  public:
    Renderer(ShaderProgram &ShaderProgram);

  protected:
    void setIndices(std::vector<GLuint> const &indices);
    void setVertices(std::vector<glm::vec3> const &vertices);
    void setModelMatrix(glm::mat4 const &modelMatrix);
    void setColour(glm::vec3 const &colour);
    void setBackColour(glm::vec3 const &colour);
    void draw(GLenum mode);
    void draw(GLenum mode, int start, int count);

  private:
    GLuint vao, vbo, ibo;
    ShaderProgram shaderProgram;
    glm::mat4 modelMatrix;
    glm::vec3 colour, backColour;
    int indexSize;
};

class ClothRenderer : public Renderer {
  public:
    ClothRenderer(int n, int m, glm::vec3 frontColour, glm::vec3 backColour,
                  ShaderProgram &shaderProgram);
    void render(std::vector<std::vector<glm::vec3>> const &mesh);

  private:
    glm::vec3 frontColour, backColour;
};

class MeshRenderer : public Renderer {
  public:
    MeshRenderer(int n, int m, glm::vec3 nodesColour, glm::vec3 linksColour,
                 ShaderProgram &shaderProgram);
    void render(std::vector<std::vector<glm::vec3>> const &mesh);

  private:
    int n, m;
    glm::vec3 nodesColour, linksColour;
};

class CuboidRenderer : public Renderer {
  public:
    CuboidRenderer(ShaderProgram &shaderProgram);
    void render(glm::vec3 colour, glm::mat4 const &modelMatrix);
};

class BallRenderer : public Renderer {
  public:
    BallRenderer(ShaderProgram &shaderProgram);
    void render(glm::vec3 colour, glm::mat4 const &modelMatrix);

  private:
    static constexpr int N = 100;
};

#endif // RENDERER_HPP

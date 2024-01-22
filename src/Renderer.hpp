#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Shader.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Renderer {
  public:
    Renderer(ShaderProgram &ShaderProgram, glm::mat4 modelMatrix);

  protected:
    void setIndices(std::vector<GLuint> const &indices);
    void setVertices(std::vector<glm::vec3> const &vertices);
    void draw(GLenum mode);
    void draw(GLenum mode, int start, int count);

  private:
    GLuint vao, vbo, ibo;
    ShaderProgram shaderProgram;
    glm::mat4 modelMatrix;
    int indexSize;
};

class ClothRenderer : public Renderer {
  public:
    ClothRenderer(int n, int m, ShaderProgram &shaderProgram);
    void render(std::vector<std::vector<glm::vec3>> const &mesh);
};

class MeshRenderer : public Renderer {
  public:
    MeshRenderer(int n, int m, ShaderProgram &shaderProgram);
    void render(std::vector<std::vector<glm::vec3>> const &mesh);

  private:
    int n, m;
};

#endif // RENDERER_HPP

#include "Renderer.hpp"
#include "Settings.hpp"
#include "glError.hpp"
#include <iostream>

Renderer::Renderer(ShaderProgram &shaderProgram)
    : shaderProgram(shaderProgram) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    shaderProgram.setAttribute("in_Position", 3, sizeof(glm::vec3), 0);

    glGenBuffers(1, &ibo);
    glBindVertexArray(0);
}

void Renderer::setIndices(std::vector<GLuint> const &indices) {
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                 indices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
    indexSize = indices.size();
}

void Renderer::setVertices(std::vector<glm::vec3> const &vertices) {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
                 vertices.data(), GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
}

void Renderer::setModelMatrix(glm::mat4 const &modelMatrix) {
    this->modelMatrix = modelMatrix;
}

void Renderer::draw(GLenum mode, int start, int count) {
    shaderProgram.use();
    shaderProgram.setUniform("modelMatrix", modelMatrix);
    glBindVertexArray(vao);
    glDrawElements(mode, count, GL_UNSIGNED_INT,
                   reinterpret_cast<void *>(start * sizeof(GLuint)));
    glCheckError(__FILE__, __LINE__);
    glBindVertexArray(0);
    shaderProgram.unuse();
}

void Renderer::draw(GLenum mode) {
    draw(mode, 0, indexSize);
}

ClothRenderer::ClothRenderer(int n, int m, ShaderProgram &shaderProgram)
    : Renderer(shaderProgram) {
    std::vector<GLuint> index;
    for (int y = 0; y < n - 1; ++y) {
        for (int x = 0; x < m - 1; ++x) {
            index.push_back((x + 0) + m * (y + 0));
            index.push_back((x + 1) + m * (y + 1));
            index.push_back((x + 1) + m * (y + 0));

            index.push_back((x + 1) + m * (y + 1));
            index.push_back((x + 0) + m * (y + 0));
            index.push_back((x + 0) + m * (y + 1));
        }
    }
    setIndices(index);
}

void ClothRenderer::render(std::vector<std::vector<glm::vec3>> const &mesh) {
    if (settings.renderMode != Settings::RenderMode::Triangles)
        return;

    setModelMatrix(glm::mat4(1.0));
    std::vector<glm::vec3> vertices;
    for (auto const &row : mesh)
        for (auto const &vertex : row)
            vertices.push_back(vertex);
    setVertices(vertices);
    draw(GL_TRIANGLES);
}

MeshRenderer::MeshRenderer(int n, int m, ShaderProgram &shaderProgram)
    : Renderer(shaderProgram), n(n), m(m) {
    std::vector<GLuint> index;

    // points
    for (int y = 0; y < n; ++y)
        for (int x = 0; x < m; ++x)
            index.push_back(x + m * y);

    // regular links
    for (int y = 0; y < n; ++y) {
        for (int x = 0; x < m; ++x) {
            if (y + 1 < n) {
                index.push_back(x + m * y);
                index.push_back(x + m * (y + 1));
            }

            if (x + 1 < m) {
                index.push_back(x + m * y);
                index.push_back((x + 1) + m * y);
            }
        }
    }

    // diagonal links
    for (int y = 0; y + 1 < n; ++y) {
        for (int x = 0; x + 1 < m; ++x) {
            index.push_back(x + m * y);
            index.push_back((x + 1) + m * (y + 1));

            index.push_back((x + 1) + m * y);
            index.push_back(x + m * (y + 1));
        }
    }

    setIndices(index);
}

void MeshRenderer::render(std::vector<std::vector<glm::vec3>> const &mesh) {
    if (settings.renderMode == Settings::RenderMode::Triangles)
        return;

    setModelMatrix(glm::mat4(1.0));
    std::vector<glm::vec3> vertices;
    for (auto const &row : mesh)
        for (auto const &vertex : row)
            vertices.push_back(vertex);
    setVertices(vertices);
    glPointSize(3.F);
    draw(GL_POINTS, 0, n * m);

    if (settings.renderMode == Settings::RenderMode::Lines) {
        glLineWidth(1.F);

        if (settings.regularLinks)
            draw(GL_LINES, n * m, 2 * (n * (m - 1) + m * (n - 1)));
        if (settings.diagonalLinks)
            draw(GL_LINES, n * m + 2 * (n * (m - 1) + m * (n - 1)),
                 4 * (n - 1) * (m - 1));
    }
}

CuboidRenderer::CuboidRenderer(ShaderProgram &shaderProgram)
    : Renderer(shaderProgram) {
    setIndices({0, 2, 1, 3, 5, 7, 4, 6, 1, 5, 0, 4, 2, 6, 3, 7});
    setVertices({glm::vec3(-1, -1, -1), glm::vec3(-1, -1, 1),
                 glm::vec3(-1, 1, -1), glm::vec3(-1, 1, 1),
                 glm::vec3(1, -1, -1), glm::vec3(1, -1, 1), glm::vec3(1, 1, -1),
                 glm::vec3(1, 1, 1)});
}

void CuboidRenderer::render(glm::mat4 const &modelMatrix) {
    setModelMatrix(modelMatrix);
    draw(GL_TRIANGLE_STRIP, 0, 8);
    draw(GL_TRIANGLE_STRIP, 8, 8);
}

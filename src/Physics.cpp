#include "Physics.hpp"

#include "Settings.hpp"
#include "asset.hpp"
#include "glError.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

// TODO: dac mozliwosc lapania myszka
// TODO: dac mozliwosc zmiany rozmiaru siatki? -> to wymaga resetu

CuboidRenderer *Cuboid::renderer = nullptr;

Cuboid::Cuboid(glm::vec3 a, glm::vec3 b, glm::vec3 colour)
    : a(a), b(b), colour(colour) {
    glm::mat4 scale = glm::scale(glm::mat4(1.0), (b - a) / 2.0F);
    glm::mat4 translate = glm::translate(glm::mat4(1.0), (a + b) / 2.0F);
    modelMatrix = translate * scale;
}

void Cuboid::initRenderer(ShaderProgram shaderProgram) {
    renderer = new CuboidRenderer(shaderProgram);
}

void Cuboid::draw() {
    renderer->render(colour, modelMatrix);
}

void Cuboid::constrain(Node &node) {
    static const float eps = 0.03F;
    auto pos = node.position;
    if (pos.x < a.x - eps || pos.x > b.x + eps || pos.y < a.y - eps || pos.y > b.y + eps ||
        pos.z < a.z - eps || pos.z > b.z + eps)
        return;

    auto candidates = {
        glm::vec3(a.x - eps, pos.y, pos.z), glm::vec3(b.x + eps, pos.y, pos.z),
        glm::vec3(pos.x, a.y - eps, pos.z), glm::vec3(pos.x, b.y + eps, pos.z),
        glm::vec3(pos.x, pos.y, a.z - eps), glm::vec3(pos.x, pos.y, b.z + eps),
    };

    float minDist = 1e9;
    glm::vec3 minPos;
    for (auto candidate : candidates) {
        float dist = glm::length(candidate - pos);
        if (dist < minDist) {
            minDist = dist;
            minPos = candidate;
        }
    }

    node.position = minPos;
    node.prevPosition = minPos;
}

const glm::vec3 G(0, 0, -9.81);

Node::Node(glm::vec3 position, glm::vec3 velocity)
    : position(position), prevPosition(position - velocity) {
}

void Node::update(float dt, float prevDt) { // TODO: sprobowac zalozyc dt=prevDt
    glm::vec3 dp = G * dt * dt;
    if (prevDt != 0.) {
        dp += (position - prevPosition) * dt / prevDt;
    }

    glm::vec3 tmp = position;
    position +=
        (1.0F - AIR_RESISTANCE) * dp; // TODO: zeby dzialalo dla malego dt
    prevPosition = tmp;
}

void Node::collide(Node &other) {
    glm::vec3 diff = position - other.position;
    float length = 0.4F; // TODO: uzaleznic od rozmiaru siatki!!!!
    float currLen = glm::length(diff);
    if (currLen > length)
        return;
    float perc = (currLen - length) / currLen * 0.4F;
    glm::vec3 off = diff * perc;
    position -= off;
    other.position += off;
}

// quite poor one...
void Node::constrainBall(glm::vec3 center, float radius) {
    glm::vec3 diff = position - center;
    float len = glm::length(diff);
    if (len < radius) {
        position += diff * (radius - len) / len;
        prevPosition = position;
        // position = prevPosition;
    }
}

Link::Link(Node *a, Node *b)
    : a(a), b(b), length(glm::length(a->position - b->position)) {
}

void Link::update() {
    glm::vec3 diff = a->position - b->position;
    float currLen = glm::length(diff);
    float perc = (currLen - length) / currLen * STIFFNESS;
    glm::vec3 off = diff * perc;
    a->position -= off;
    b->position += off;
}

Cloth::Cloth(glm::vec3 pos, glm::vec3 _dx, glm::vec3 _dy, int width, int height,
             glm::vec3 nodesColour, glm::vec3 linksColour,
             glm::vec3 frontColour, glm::vec3 backColour,
             TrianglesShaderProgram &trianglesShaderProgram,
             PointsAndLinesShaderProgram &pointsAndLinesShaderProgram)
    : pos(pos), dx(_dx / static_cast<float>(width)),
      dy(_dy / static_cast<float>(height)),
      clothRenderer(SUBDIVISION_MESH_SIZE_MAX, SUBDIVISION_MESH_SIZE_MAX,
                    frontColour, backColour, trianglesShaderProgram),
      meshRenderer(height + 1, width + 1, nodesColour, linksColour,
                   pointsAndLinesShaderProgram) {
    nodes.resize(height + 1);
    for (int y = 0; y <= height; ++y) {
        for (int x = 0; x <= width; ++x) {
            glm::vec3 xx = dx * static_cast<float>(x);
            glm::vec3 yy = dy * static_cast<float>(y);
            nodes[y].emplace_back(pos + xx + yy);
        }
    }

    for (int y = 0; y <= height; ++y) {
        for (int x = 0; x <= width; ++x) {
            if (x < width)
                regularLinks.push_back(Link(&nodes[y][x], &nodes[y][x + 1]));
            if (y < height)
                regularLinks.push_back(Link(&nodes[y][x], &nodes[y + 1][x]));
            if (x < width && y < height) {
                diagonalLinks.push_back(
                    Link(&nodes[y][x], &nodes[y + 1][x + 1]));
                diagonalLinks.push_back(
                    Link(&nodes[y][x + 1], &nodes[y + 1][x]));
            }
            if (x < width - 1)
                farLinks.push_back(Link(&nodes[y][x], &nodes[y][x + 2]));
            if (y < height - 1)
                farLinks.push_back(Link(&nodes[y][x], &nodes[y + 2][x]));
        }
    }

    settings.registerSubdivisionStepsCallback(
        [this, frontColour, backColour, &trianglesShaderProgram]() {
            int n = nodes.size();
            int m = nodes[0].size();
            for (int i = 0; i < settings.subdivisionSteps; i++) {
                n = n * 2 - 1;
                m = m * 2 - 1;
            }
            clothRenderer = ClothRenderer(n, m, frontColour, backColour,
                                          trianglesShaderProgram);
        });
}

void Cloth::reset() {
    for (size_t y = 0; y < nodes.size(); ++y) {
        for (size_t x = 0; x < nodes[y].size(); ++x) {
            glm::vec3 xx = dx * static_cast<float>(x);
            glm::vec3 yy = dy * static_cast<float>(y);
            nodes[y][x].position = pos + xx + yy;
            nodes[y][x].prevPosition = nodes[y][x].position;
        }
    }
}

void Cloth::update(float dt, float prevDt, std::vector<Solid *> const &solids) {
    for (auto &line : nodes)
        for (auto &node : line)
            node.update(dt, prevDt);

    for (int t = 0; t < 5; t++) {
        if (settings.regularLinks)
            for (auto &link : regularLinks)
                link.update();

        if (settings.diagonalLinks)
            for (auto &link : diagonalLinks)
                link.update();

        if (settings.farLinks)
            for (auto &link : farLinks)
                link.update();

        // for (auto &line : nodes)
        //     for (auto &node : line)
        //         node.constrainBall(glm::vec3(0, 0, 2), 1.0);

        for (auto solid : solids)
            for (auto &line : nodes)
                for (auto &node : line)
                    solid->constrain(node);

        if (settings.clothClothCollision) {
            for (int i = 0; i < static_cast<int>(nodes.size()); i++) {
                for (int j = 0; j < static_cast<int>(nodes[i].size()); j++) {
                    for (int k = 0; k < static_cast<int>(nodes.size()); k++) {
                        for (int l = 0; l < static_cast<int>(nodes[k].size());
                             l++) {
                            if (abs(i - k) > 1 || abs(j - l) > 1)
                                nodes[i][j].collide(nodes[k][l]);
                        }
                    }
                }
            }
        }
    }
}

std::vector<std::vector<glm::vec3>>
catmullClarkStep(std::vector<std::vector<glm::vec3>> grid) {
    size_t n = grid.size();
    size_t m = grid[0].size();

    std::vector<std::vector<glm::vec3>> outGrid(n * 2 - 1);
    for (size_t i = 0; i < outGrid.size(); i++)
        outGrid[i].resize(m * 2 - 1);

    for (size_t i = 0; i + 1 < n; i++)
        for (size_t j = 0; j + 1 < m; j++)
            outGrid[2 * i + 1][2 * j + 1] =
                1.F / 4 * grid[i][j] + 1.F / 4 * grid[i][j + 1] +
                1.F / 4 * grid[i + 1][j] + 1.F / 4 * grid[i + 1][j + 1];

    for (size_t i = 1; i + 1 < n; i++)
        for (size_t j = 0; j + 1 < m; j++)
            outGrid[2 * i][2 * j + 1] =
                1.F / 16 * grid[i - 1][j] + 3.F / 8 * grid[i][j] +
                1.F / 16 * grid[i + 1][j] + 1.F / 16 * grid[i - 1][j + 1] +
                3.F / 8 * grid[i][j + 1] + 1.F / 16 * grid[i + 1][j + 1];

    for (size_t i = 0; i + 1 < n; i++)
        for (size_t j = 1; j + 1 < m; j++)
            outGrid[2 * i + 1][2 * j] =
                1.F / 16 * grid[i][j - 1] + 3.F / 8 * grid[i][j] +
                1.F / 16 * grid[i][j + 1] + 1.F / 16 * grid[i + 1][j - 1] +
                3.F / 8 * grid[i + 1][j] + 1.F / 16 * grid[i + 1][j + 1];

    for (size_t i = 1; i + 1 < n; i++)
        for (size_t j = 1; j + 1 < m; j++)
            outGrid[2 * i][2 * j] =
                1.F / 64 * grid[i - 1][j - 1] + 3.F / 32 * grid[i - 1][j] +
                1.F / 64 * grid[i - 1][j + 1] + 3.F / 32 * grid[i][j - 1] +
                9.F / 16 * grid[i][j] + 3.F / 32 * grid[i][j + 1] +
                1.F / 64 * grid[i + 1][j - 1] + 3.F / 32 * grid[i + 1][j] +
                1.F / 64 * grid[i + 1][j + 1];

    outGrid[0][0] = grid[0][0];
    outGrid[0][(m - 1) * 2] = grid[0][m - 1];
    outGrid[(n - 1) * 2][0] = grid[n - 1][0];
    outGrid[(n - 1) * 2][(m - 1) * 2] = grid[n - 1][m - 1];

    for (size_t i = 1; i + 1 < n; i++) {
        outGrid[2 * i][0] = 1.F / 8 * grid[i - 1][0] + 3.F / 4 * grid[i][0] +
                            1.F / 8 * grid[i + 1][0];
        outGrid[2 * i][(m - 1) * 2] = 1.F / 8 * grid[i - 1][m - 1] +
                                      3.F / 4 * grid[i][m - 1] +
                                      1.F / 8 * grid[i + 1][m - 1];
    }

    for (size_t j = 1; j + 1 < m; j++) {
        outGrid[0][2 * j] = 1.F / 8 * grid[0][j - 1] + 3.F / 4 * grid[0][j] +
                            1.F / 8 * grid[0][j + 1];
        outGrid[(n - 1) * 2][2 * j] = 1.F / 8 * grid[n - 1][j - 1] +
                                      3.F / 4 * grid[n - 1][j] +
                                      1.F / 8 * grid[n - 1][j + 1];
    }

    for (size_t i = 0; i + 1 < n; i++) {
        outGrid[2 * i + 1][0] = 1.F / 2 * grid[i][0] + 1.F / 2 * grid[i + 1][0];
        outGrid[2 * i + 1][(m - 1) * 2] =
            1.F / 2 * grid[i][m - 1] + 1.F / 2 * grid[i + 1][m - 1];
    }

    for (size_t j = 0; j + 1 < m; j++) {
        outGrid[0][2 * j + 1] = 1.F / 2 * grid[0][j] + 1.F / 2 * grid[0][j + 1];
        outGrid[(n - 1) * 2][2 * j + 1] =
            1.F / 2 * grid[n - 1][j] + 1.F / 2 * grid[n - 1][j + 1];
    }

    return outGrid;
}

std::vector<std::vector<glm::vec3>>
catmullClark(std::vector<std::vector<glm::vec3>> grid, int n) {
    for (int i = 0; i < n; i++)
        grid = catmullClarkStep(grid);
    return grid;
}

void Cloth::draw() {
    std::vector<std::vector<glm::vec3>> grid(nodes.size());

    for (size_t i = 0; i < nodes.size(); i++) {
        grid[i].resize(nodes[i].size());
        for (size_t j = 0; j < nodes[i].size(); j++) {
            grid[i][j] = nodes[i][j].position;
        }
    }

    clothRenderer.render(catmullClark(grid, settings.subdivisionSteps));
    meshRenderer.render(grid);
}

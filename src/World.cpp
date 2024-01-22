#include "World.hpp"
#include "Settings.hpp"
#include "Shader.hpp"

World::World(TrianglesShaderProgram &trianglesShaderProgram,
             PointsAndLinesShaderProgram &pointsAndLinesShaderProgram)
    : cloth(glm::vec3(-5.0, -5.0, 5.0), glm::vec3(10.0, 0.0, 0.0),
            glm::vec3(0.0, 10.0, 0.0), 30, 30, trianglesShaderProgram,
            pointsAndLinesShaderProgram) {
    settings.registerResetCallback([this]() {
        reset();
    });
}

void World::reset() {
    cloth.reset();
}

void World::update(float dt, float prevDt) {
    cloth.update(dt, prevDt);
}

void World::draw() {
    cloth.draw();
}

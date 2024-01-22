#include "World.hpp"
#include "Settings.hpp"
#include "Shader.hpp"

World::World(TrianglesShaderProgram &trianglesShaderProgram,
             PointsAndLinesShaderProgram &pointsAndLinesShaderProgram) {
    settings.registerResetCallback([this]() {
        reset();
    });

    cloth = new Cloth(glm::vec3(-5.0, -5.0, 5.0), // pos
                      glm::vec3(10.0, 0.0, 0.0),  // dx
                      glm::vec3(0.0, 10.0, 0.0),  // dy
                      30,                         // width
                      30,                         // height
                      glm::vec3(0.8, 0.0, 0.0),   // nodesColour
                      glm::vec3(0.0, 0.0, 0.6),   // linksColour
                      glm::vec3(0.9, 0.9, 0.9),   // frontColour
                      glm::vec3(0.15, 0.2, 0.6),  // backColour
                      trianglesShaderProgram, pointsAndLinesShaderProgram);

    solids.push_back(new Cuboid(glm::vec3(-5.0, -7.0, -3.0), // a
                                glm::vec3(7.0, 7.0, -2.5),   // b
                                glm::vec3(0.15, 0.1, 0.1)    // colour
                                ));

    Cuboid::initRenderer(trianglesShaderProgram);
}

void World::reset() {
    cloth->reset();
}

void World::update(float dt, float prevDt) {
    cloth->update(dt, prevDt);
}

void World::draw() {
    cloth->draw();
    for (auto solid : solids) {
        solid->draw();
    }
}

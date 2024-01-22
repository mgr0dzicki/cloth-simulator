#include "World.hpp"
#include "Settings.hpp"
#include "Shader.hpp"

Cloth *createCloth(TrianglesShaderProgram &trianglesShaderProgram,
                   PointsAndLinesShaderProgram &pointsAndLinesShaderProgram) {
    return new Cloth(glm::vec3(-5.0, -5.0, 5.0), // pos
                     glm::vec3(10.0, 0.0, 0.0),  // dx
                     glm::vec3(0.0, 10.0, 0.0),  // dy
                     glm::vec3(0.8, 0.0, 0.0),   // nodesColour
                     glm::vec3(0.0, 0.0, 0.6),   // linksColour
                     glm::vec3(0.9, 0.9, 0.9),   // frontColour
                     glm::vec3(0.15, 0.2, 0.6),  // backColour
                     trianglesShaderProgram, pointsAndLinesShaderProgram);
}

World::World(TrianglesShaderProgram &trianglesShaderProgram,
             PointsAndLinesShaderProgram &pointsAndLinesShaderProgram)
    : trianglesShaderProgram(trianglesShaderProgram),
      pointsAndLinesShaderProgram(pointsAndLinesShaderProgram) {
    settings.registerResetCallback([this]() {
        reset();
    });

    cloth = createCloth(trianglesShaderProgram, pointsAndLinesShaderProgram);

    solids.push_back(new Cuboid(glm::vec3(-5.0, -7.0, -3.0), // a
                                glm::vec3(7.0, 7.0, -2.5),   // b
                                glm::vec3(0.15, 0.1, 0.1)    // colour
                                ));
    solids.push_back(new Ball(glm::vec3(0.0, 0.0, 1.5), // center
                              1.0,                      // radius
                              glm::vec3(0.3, 0.15, 0.0) // colour
                              ));
    solids.push_back(new Ball(glm::vec3(-3.0, -3.0, 0.0), // center
                              1.5,                        // radius
                              glm::vec3(0.3, 0.15, 0.0)   // colour
                              ));
    solids.push_back(new Ball(glm::vec3(3.0, 3.0, -2.0), // center
                              1.5,                       // radius
                              glm::vec3(0.3, 0.15, 0.0)  // colour
                              ));
    solids.push_back(new Ball(glm::vec3(0.0, 0.0, -2.5), // center
                              0.5,                       // radius
                              glm::vec3(0.3, 0.15, 0.0)  // colour
                              ));
    solids.push_back(new Ball(glm::vec3(2.0, -2.0, -1.5), // center
                              1.0,                        // radius
                              glm::vec3(0.3, 0.15, 0.0)   // colour
                              ));

    Cuboid::initRenderer(trianglesShaderProgram);
    Ball::initRenderer(trianglesShaderProgram);
}

void World::reset() {
    delete cloth;
    cloth = createCloth(trianglesShaderProgram, pointsAndLinesShaderProgram);
}

void World::update(float dt, float prevDt) {
    cloth->update(dt, prevDt, solids);
}

void World::draw() {
    cloth->draw();
    for (auto solid : solids) {
        solid->draw();
    }
}

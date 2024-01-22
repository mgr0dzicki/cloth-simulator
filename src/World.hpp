#include "Physics.hpp"
#include "Shader.hpp"
#ifndef WORLD_HPP
#define WORLD_HPP

class World {
  public:
    World(TrianglesShaderProgram &trianglesShaderProgram,
          PointsAndLinesShaderProgram &pointsAndLinesShaderProgram);
    void reset();
    void update(float dt, float prevDt);
    void draw();

  private:
    Cloth *cloth;
    std::vector<Solid *> solids;
};

#endif // WORLD_HPP

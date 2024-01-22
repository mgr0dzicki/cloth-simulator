/**
 * MyApplication.hpp skeleton
 * Contributors:
 *      * Arthur Sonzogni
 * Licence:
 *      * MIT
 */

#ifndef MYAPPLICATION_HPP
#define MYAPPLICATION_HPP

#include "Application.hpp"
#include "Cloth.hpp"
#include "SettingsWindow.hpp"
#include "Shader.hpp"
#include "World.hpp"

class MyApplication : public Application {
  public:
    MyApplication();

  protected:
    virtual void loop();

  private:
    const int size = 30;

    // shader
    TrianglesShaderProgram trianglesShaderProgram;
    PointsAndLinesShaderProgram pointsAndLinesShaderProgram;

    // shader matrix uniform
    glm::mat4 viewProjectionMatrix;

    // world
    World world;

    // time
    float prevDeltaTime = 0.0;
};

#endif // MYAPPLICATION_HPP

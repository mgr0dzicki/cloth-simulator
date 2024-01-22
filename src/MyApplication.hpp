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
#include "Physics.hpp"
#include "SettingsWindow.hpp"
#include "Shader.hpp"
#include "World.hpp"

class MyApplication : public Application {
  public:
    MyApplication();

  protected:
    virtual void loop();

  private:
    // shader
    TrianglesShaderProgram trianglesShaderProgram;
    PointsAndLinesShaderProgram pointsAndLinesShaderProgram;

    // world
    World world;

    // time
    float prevDeltaTime = 0.0;

    // matrices
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 cameraPosition = glm::vec3(30.0, 0.0, 5.0);

    // mouse callbacks
    bool turning = false;
    double prevMouseX, prevMouseY;
    void onMouseButton(int button, int action, int mods, double x,
                       double y) override;
    void onMouseMove(double x, double y) override;
};

#endif // MYAPPLICATION_HPP

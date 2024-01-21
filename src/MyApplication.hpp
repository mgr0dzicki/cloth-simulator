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

class MyApplication : public Application {
  public:
    MyApplication();

  protected:
    virtual void loop();
    virtual void onMouseMove(double x, double y);
    virtual void onMouseButton(int button, int action, int mods, double x,
                               double y);

  private:
    const int size = 30;

    // shader
    Shader vertexShader;
    Shader fragmentShader;
    Shader geometryShader;
    ShaderProgram shaderProgram;

    // shader matrix uniform
    glm::mat4 viewProjectionMatrix;

    // cloth
    Cloth cloth;

    // time
    float prevDeltaTime = 0.0;

    // state
    bool movingCloth = false;
};

#endif // MYAPPLICATION_HPP

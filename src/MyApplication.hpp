/**
 * MyApplication.hpp skeleton
 * Contributors:
 *      * Arthur Sonzogni
 * Licence:
 *      * MIT
 */

#ifndef OPENGL_CMAKE_SKELETON_MYAPPLICATION
#define OPENGL_CMAKE_SKELETON_MYAPPLICATION

#include "Application.hpp"
#include "Cloth.hpp"
#include "SettingsWindow.hpp"
#include "Shader.hpp"

class MyApplication : public Application {
  public:
    MyApplication();

  protected:
    virtual void loop();

  private:
    const int size = 30;

    // shader
    Shader vertexShader;
    Shader fragmentShader;
    Shader geometryShader;
    ShaderProgram shaderProgram;

    // shader matrix uniform
    glm::mat4 projection = glm::mat4(1.0);
    glm::mat4 view = glm::mat4(1.0);
    glm::mat4 model = glm::mat4(1.0);

    // cloth
    Cloth cloth;

    // time
    float prevDeltaTime = 0.0;

    // settings
    SettingsWindow settingsWindow;
};

#endif // OPENGL_CMAKE_SKELETON_MYAPPLICATION

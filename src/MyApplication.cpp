#include "MyApplication.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <iostream>
#include <vector>

#include "SettingsWindow.hpp"
#include "asset.hpp"
#include "glError.hpp"

MyApplication::MyApplication()
    : Application("Cloth simulator"),
      cloth(glm::vec3(-5.0, -5.0, 5.0), glm::vec3(10.0, 0.0, 0.0),
            glm::vec3(0.0, 10.0, 0.0), size, size, trianglesShaderProgram,
            pointsAndLinesShaderProgram) {

    glCheckError(__FILE__, __LINE__);

    glm::mat4 projection =
        glm::perspective(0.4f, getWindowRatio(), 0.1f, 100.f);
    glm::mat4 view =
        glm::lookAt(glm::vec3(20.0, 0.0, 20.0), glm::vec3(0.0, 0.0, 0.0),
                    glm::vec3(0.0, 0.0, 1.0));
    viewProjectionMatrix = projection * view;

    // send uniforms
    trianglesShaderProgram.use();
    trianglesShaderProgram.setUniform("viewProjectionMatrix",
                                      viewProjectionMatrix);

    trianglesShaderProgram.setUniform("lightPosition",
                                      glm::vec4(20.0, 0.0, 10.0, 1.0));
    trianglesShaderProgram.setUniform("lightDirection",
                                      glm::vec3(0.8, 0.8, 0.8));
    trianglesShaderProgram.setUniform("lightAmbient",
                                      glm::vec3(0.1, 0.1, 0.15));
    trianglesShaderProgram.setUniform("lightAttenuation",
                                      glm::vec3(1.0, 0.0, 0.0));
    trianglesShaderProgram.unuse();

    glCheckError(__FILE__, __LINE__);

    pointsAndLinesShaderProgram.use();
    pointsAndLinesShaderProgram.setUniform("viewProjectionMatrix",
                                           viewProjectionMatrix);
    pointsAndLinesShaderProgram.unuse();

    glCheckError(__FILE__, __LINE__);
}

void MyApplication::loop() {
    // exit on window close button pressed
    if (glfwWindowShouldClose(getWindow()))
        exit();

    // float t = getTime();
    float dt = getFrameDeltaTime();
    float prevDt = prevDeltaTime;

    drawSettingsWindow(dt);

    // clear
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glCheckError(__FILE__, __LINE__);

    // draw
    cloth.update(dt, prevDt);
    prevDeltaTime = dt;
    cloth.draw();

    glCheckError(__FILE__, __LINE__);
}

void MyApplication::onMouseButton(int button, int action, int mods, double x,
                                  double y) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        movingCloth = true;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        movingCloth = false;
    }
}

void MyApplication::onMouseMove(double x, double y) {
    if (movingCloth) {
    }
}

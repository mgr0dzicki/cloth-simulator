#include "MyApplication.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <iostream>
#include <vector>

#include "Settings.hpp"
#include "SettingsWindow.hpp"
#include "asset.hpp"
#include "glError.hpp"

MyApplication::MyApplication()
    : Application("Cloth simulator"),
      world(trianglesShaderProgram, pointsAndLinesShaderProgram) {

    glCheckError(__FILE__, __LINE__);

    // send uniforms

    view = glm::lookAt(cameraPosition, glm::vec3(0.0, 0.0, 0.0),
                       glm::vec3(0.0, 0.0, 1.0));

    trianglesShaderProgram.use();
    trianglesShaderProgram.setUniform("lightPosition",
                                      glm::vec4(2.0, 2.0, 8.0, 1.0));
    trianglesShaderProgram.setUniform("lightDirection",
                                      glm::vec3(0.8, 0.8, 0.8));
    trianglesShaderProgram.setUniform("lightAmbient",
                                      glm::vec3(0.1, 0.1, 0.15));
    trianglesShaderProgram.setUniform("lightAttenuation",
                                      glm::vec3(1.0, 0.0, 0.0));
    trianglesShaderProgram.unuse();

    glCheckError(__FILE__, __LINE__);
}

void MyApplication::loop() {
    // exit on window close button pressed
    if (glfwWindowShouldClose(getWindow()))
        exit();

    projection = glm::perspective(0.5f, getWindowRatio(), 0.1f, 100.f);
    glm::mat4 viewProjectionMatrix = projection * view;

    trianglesShaderProgram.use();
    trianglesShaderProgram.setUniform("viewProjectionMatrix",
                                      viewProjectionMatrix);
    trianglesShaderProgram.unuse();

    pointsAndLinesShaderProgram.use();
    pointsAndLinesShaderProgram.setUniform("viewProjectionMatrix",
                                           viewProjectionMatrix);
    pointsAndLinesShaderProgram.unuse();

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
    if (!settings.frozen) {
        world.update(dt, prevDt);
        prevDeltaTime = dt;
    }
    world.draw();

    glCheckError(__FILE__, __LINE__);
}

void MyApplication::onMouseButton(int button, int action, int mods, double x,
                                  double y) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            turning = true;
            prevMouseX = x;
            prevMouseY = y;
        } else if (action == GLFW_RELEASE) {
            turning = false;
        }
    }
}

void MyApplication::onMouseMove(double x, double y) {
    if (turning) {
        double dx = (x - prevMouseX) / getWidth() * 4.0;
        double dy = (y - prevMouseY) / getHeight() * 4.0;
        prevMouseX = x;
        prevMouseY = y;

        glm::vec3 up = glm::vec3(0.0, 0.0, 1.0);
        glm::vec3 right = glm::cross(cameraPosition, up);
        glm::mat4 rotation =
            glm::rotate(glm::mat4(1.0), float(-dx), glm::vec3(0.0, 0.0, 1.0));
        rotation = glm::rotate(rotation, float(dy), right);
        cameraPosition = glm::vec3(rotation * glm::vec4(cameraPosition, 1.0));
        view = glm::lookAt(cameraPosition, glm::vec3(0.0, 0.0, 0.0), up);
    }
}

void MyApplication::onScroll(double x, double y) {
    cameraPosition *= 1.0 - y / 20.0;
    view = glm::lookAt(cameraPosition, glm::vec3(0.0, 0.0, 0.0),
                       glm::vec3(0.0, 0.0, 1.0));
}

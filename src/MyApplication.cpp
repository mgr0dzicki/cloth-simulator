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

    trianglesShaderProgram.use();
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
}

void MyApplication::loop() {
    // exit on window close button pressed
    if (glfwWindowShouldClose(getWindow()))
        exit();

    glm::mat4 projection =
        glm::perspective(0.5f, getWindowRatio(), 0.1f, 100.f);
    glm::mat4 view =
        glm::lookAt(glm::vec3(30.0, 0.0, 5.0), glm::vec3(0.0, 0.0, 0.0),
                    glm::vec3(0.0, 0.0, 1.0));
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

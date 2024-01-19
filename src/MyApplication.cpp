#include "MyApplication.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <iostream>
#include <vector>

#include "asset.hpp"
#include "glError.hpp"

MyApplication::MyApplication()
    : Application(),
      vertexShader(SHADER_DIR "/shader.vert", GL_VERTEX_SHADER),
      fragmentShader(SHADER_DIR "/shader.frag", GL_FRAGMENT_SHADER),
      geometryShader(SHADER_DIR "/shader.geom", GL_GEOMETRY_SHADER),
      shaderProgram({vertexShader, fragmentShader, geometryShader}),
      cloth(glm::vec3(-5.0, -5.0, 5.0), glm::vec3(10.0, 0.0, 0.0),
            glm::vec3(0.0, 10.0, 0.0), size, size, shaderProgram) {
  glCheckError(__FILE__, __LINE__);
}

void MyApplication::loop() {
  // exit on window close button pressed
  if (glfwWindowShouldClose(getWindow()))
    exit();

  // float t = getTime();
  float dt = getFrameDeltaTime();
  std::cout << "dt=" << dt << std::endl;
  float prevDt = prevDeltaTime;
  // set matrix : projection + view
  projection = glm::perspective(0.4f, getWindowRatio(), 0.1f, 100.f);
  view = glm::lookAt(glm::vec3(20.0, 0.0, 20.0),
                     glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));

  // clear
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shaderProgram.use();

  // send uniforms
  shaderProgram.setUniform("modelMatrix", model);
  shaderProgram.setUniform("viewProjectionMatrix", projection * view);

  shaderProgram.setUniform("lightPosition", glm::vec4(-5.0, -5.0, 10.0, 1.0));
  shaderProgram.setUniform("lightDirection", glm::vec3(0.8, 0.8, 0.8));
  shaderProgram.setUniform("lightAmbient", glm::vec3(0.1, 0.1, 0.15));
  shaderProgram.setUniform("lightAttenuation", glm::vec3(1.0, 0.0, 0.0));

  glCheckError(__FILE__, __LINE__);

  // draw
  cloth.update(dt, prevDt);
  prevDeltaTime = dt;
  cloth.draw();

  shaderProgram.unuse();

  glCheckError(__FILE__, __LINE__);
}

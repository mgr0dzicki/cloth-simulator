#version 420

layout(location=0) in vec4 in_Position;
layout(location=1) in vec3 in_Colour;

out vec3 Colour;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main(void) {
  gl_Position = projection * (view * (model * in_Position));
  Colour = in_Colour;
}

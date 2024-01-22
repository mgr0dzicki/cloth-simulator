#version 420

in vec4 in_Position;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

void main(void) {
  gl_Position = viewProjectionMatrix * (modelMatrix * in_Position);
}

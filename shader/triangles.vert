#version 420

in vec4 in_Position;

out vec3 v_Position;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

void main(void) {
  vec4 Pos;

  Pos = modelMatrix * in_Position;
  v_Position = Pos.xyz / Pos.w;
  gl_Position = viewProjectionMatrix * Pos;
}

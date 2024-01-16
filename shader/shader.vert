#version 420

in vec4 in_Position;
in vec3 in_Colour;

out Vertex {
  vec3 Colour;
  vec3 Position;
} Out;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;
uniform vec4 eyePos;

void main(void) {
  vec4 Pos;

  Pos = modelMatrix * in_Position;
  Out.Position = Pos.xyz / Pos.w;
  Out.Colour = in_Colour;
  gl_Position = viewProjectionMatrix * Pos;
}

#version 420

in vec3 Colour;

out vec4 out_Colour;

void main(void) {
  out_Colour = vec4 ( Colour, 1.0 );
}

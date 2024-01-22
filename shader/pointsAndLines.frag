#version 420

out vec4 out_Colour;

uniform vec3 frontColour;

void main ( void )
{
  out_Colour = vec4 ( frontColour, 1.0 );
}

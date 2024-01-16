#version 420

in FVertex {
  vec3 Colour;
  vec3 Position;
  vec3 Normal;
} In;

out vec4 out_Colour;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;
uniform vec4 eyePos;

uniform vec4 lightPosition;
uniform vec3 lightAmbient;
uniform vec3 lightDirection;
uniform vec3 lightAttenuation;

vec3 posDifference ( vec4 p, vec3 pos, out float dist )
{
  vec3 v;

  if ( p.w != 0.0 ) {
    v = p.xyz/p.w-pos.xyz;
    dist = sqrt ( dot ( v, v ) );
  }
  else
    v = p.xyz;
  return normalize ( v );
} /*posDifference*/

float attFactor ( vec3 att, float dist )
{
  return 1.0/(((att.z*dist)+att.y)*dist+att.x);
} /*attFactor*/

vec3 LambertLighting ( void )
{
  vec3  normal, lv, vv, Colour;
  float d, dist;
  uint  i, mask;

  normal = normalize ( In.Normal );
  vv = posDifference ( eyePos, In.Position, dist );

  Colour = vec3(0.0);
  Colour += lightAmbient * In.Colour;
  lv = posDifference ( lightPosition, In.Position, dist );
  d = dot ( lv, normal );
  if ( dot ( vv, normal ) > 0.0 ) {
    if ( d > 0.0 ) {
      if ( lightPosition.w != 0.0 )
        d *= attFactor ( lightAttenuation, dist );
      Colour += (d * lightDirection) * In.Colour;
    }
  }
  else {
    if ( d < 0.0 ) {
      if ( lightPosition.w != 0.0 )
        d *= attFactor ( lightAttenuation, dist );
      Colour -= (d * lightDirection) * In.Colour;
    }
  }
  return clamp ( Colour, 0.0, 1.0 );
} /*LambertLighting*/

#define AGamma(colour) pow ( colour, vec3(256.0/563.0) )

void main ( void )
{
  out_Colour = vec4 ( AGamma ( LambertLighting () ), 1.0 );
} /*main*/

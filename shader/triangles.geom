#version 420

layout(triangles) in;
layout(triangle_strip,max_vertices=3) out;

in vec3 v_Position[];

out FVertex {
  vec3 Colour;
  vec3 Position;
  vec3 Normal;
} Out;

void main ( void )
{
  int  i;
  vec3 v1, v2, nv;

  v1 = v_Position[1] - v_Position[0];
  v2 = v_Position[2] - v_Position[0];
  nv = normalize ( cross ( v1, v2 ) );
  for ( i = 0; i < 3; i++ ) {
    gl_Position = gl_in[i].gl_Position;
    Out.Position = v_Position[i];
    Out.Normal = nv;
    // Out.Colour = In[i].Colour;
    // Out.Colour = nv;
    Out.Colour = vec3(1., 1., 1.);
    EmitVertex ();
  }
  EndPrimitive ();
} /*main*/

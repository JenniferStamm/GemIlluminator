#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 view;
uniform mat4 projectionInverse;

attribute highp vec2 a_vertex;

varying vec3 v_eye;
 
void main()
{	
    v_eye = (projectionInverse * vec4(a_vertex, 1.0, 1.0) * view).xyz;
	
    gl_Position = vec4(a_vertex, 1.0, 1.0);
}
  

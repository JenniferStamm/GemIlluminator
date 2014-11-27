#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 modelViewProjection;
attribute highp vec3 a_vertex;

varying vec3 v_vertex;

void main()
{
	gl_Position = modelViewProjection * vec4(a_vertex, 1.0);
        v_vertex = gl_Position.xyz;
}

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif
uniform mat4 u_modelViewProjection;

attribute highp vec3 a_vertex;
attribute highp vec3 a_color;

varying vec3 v_color;

void main()
{
    v_color = a_color;
    gl_Position = u_modelViewProjection * vec4(a_vertex, 1.0);
}

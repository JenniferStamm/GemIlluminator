#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 modelViewProjection;
attribute highp vec3 a_vertex;
attribute highp vec3 a_color;
attribute highp vec3 a_normal;

varying vec3 v_color;

void main()
{
    gl_Position = modelViewProjection * vec4(a_vertex, 1.0);
    v_color = a_normal;
    v_color = a_color;
}

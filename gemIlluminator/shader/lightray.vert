#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif
uniform mat4 modelViewProjection;
uniform vec3 color;

attribute highp vec3 a_vertex;

varying vec3 v_color;

void main()
{
    v_color = color;

    gl_Position = modelViewProjection * vec4(a_vertex, 1.0);
}

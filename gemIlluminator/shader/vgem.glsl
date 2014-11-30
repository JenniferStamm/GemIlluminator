#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 modelView;
attribute highp vec3 a_vertex;
attribute highp vec3 a_color;
attribute highp vec3 a_normal;

varying vec3 v_color;

const vec3 lightPos = vec3(0.5, 0.8, 1.0);
const vec3 specColor = vec3(1.0, 1.0, 1.0);

void main()
{
    gl_Position = modelView * vec4(a_vertex, 1.0);


    v_color = a_color ;
}

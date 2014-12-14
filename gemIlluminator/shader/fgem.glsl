#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

varying vec3 v_color;
varying vec3 v_lightIntensity;

const vec4 lightDirection = vec4(4.0, 8.0, 4.0, 1.0);
const vec3 specularColor = vec3(1.0, 1.0, 1.0);

void main()
{
    gl_FragColor = vec4(v_color, 1.0);
}

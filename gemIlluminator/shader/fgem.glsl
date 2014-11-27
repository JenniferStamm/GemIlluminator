#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif
varying vec3 v_vertex;

void main()
{
        gl_FragColor = vec4(v_vertex, 1.0);
}

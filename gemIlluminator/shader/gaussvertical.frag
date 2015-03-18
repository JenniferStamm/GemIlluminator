#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D u_source;
uniform float u_blurSize;

varying vec2 v_uv;

// const float kernel[9] = float[](1.0, 2.0, 3.0, 4.0, 5.0, 4.0, 3.0, 2.0, 1.0);

void main()
{
    float v;
    vec4 color = texture2D(u_source, v_uv) * 5.0;

    for (float i = 1.0; i < 5.0; i++) {
        v = v_uv.t + u_blurSize * i;
        color += texture2D(u_source, vec2(v_uv.s, v)) * (5.0 - i);
        v = v_uv.t + u_blurSize * -i;
        color += texture2D(u_source, vec2(v_uv.s, v)) * (5.0 - i);
    }

    gl_FragColor = vec4(color / 25.0);
}

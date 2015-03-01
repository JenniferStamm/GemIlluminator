#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D lightRays;

varying vec2 v_uv;

// const float kernel[9] = float[](1.0, 2.0, 3.0, 4.0, 5.0, 4.0, 3.0, 2.0, 1.0);

const float blurSize = 1.0/512.0;

void main()
{
    float v = v_uv.t + blurSize;
    vec4 color = vec4(0.0);
    color += texture2D(lightRays, vec2(v_uv.s, v)) * 5.0;

    for (float i = 1.0; i < 5.0; i++) {
        v = v_uv.t + blurSize * i;
        color += texture2D(lightRays, vec2(v_uv.s, v)) * (5.0 - i);
        v = v_uv.t + blurSize * -i;
        color += texture2D(lightRays, vec2(v_uv.s, v)) * (5.0 - i);
    }

    gl_FragColor = vec4(color / 25.0);
}

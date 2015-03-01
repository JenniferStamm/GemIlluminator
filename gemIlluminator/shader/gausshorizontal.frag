#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D lightRays;

uniform float blurSize;

varying vec2 v_uv;

// float kernel[9] = float[9](1.0, 2.0, 3.0, 4.0, 5.0, 4.0, 3.0, 2.0, 1.0);

void main()
{
    float u;
    vec4 color = texture2D(lightRays, v_uv) * 5.0;

    for (float i = 1.0; i < 5.0; i++) {
        u = v_uv.s + blurSize * i;
        color += texture2D(lightRays, vec2(u, v_uv.t)) * (5.0 - i);
        u = v_uv.s + blurSize * -i;
        color += texture2D(lightRays, vec2(u, v_uv.t)) * (5.0 - i);
    }

    gl_FragColor = vec4(color / 25.0);
}

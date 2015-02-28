#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D lightRays;

varying vec2 v_uv;

float kernel[9] = float[9](1.0, 2.0, 3.0, 4.0, 5.0, 4.0, 3.0, 2.0, 1.0);

const float blurSize = 1.0/512.0;

void main()
{
    vec4 color = vec4(0.0);
    for (int i = 0; i < kernel.length(); i++) {
        float v = v_uv.t + blurSize * (float(i) - 4.0);
        color += texture2D(lightRays, vec2(v_uv.s, v)) * kernel[i];
    }

    gl_FragColor = vec4(color / 25.0);
}

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D lightrays;

varying vec2 v_uv;

float kernel[9] = float[9](1.0, 2.0, 3.0, 4.0, 5.0, 4.0, 3.0, 2.0, 1.0);

const float blurSize = 1.0/512.0;

void main()
{
    vec3 color = vec3(0.0);
    for (int i = 0; i < kernel.length(); i++) {
        float u = v_uv.s + blurSize * (float(i) - 4.0);
        color += texture2D(lightrays, vec2(u, v_uv.t)) * kernel[i];
    }

    gl_FragColor = vec4(color / 29.0, 1.0);
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}

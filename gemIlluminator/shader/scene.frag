#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D u_sceneTexture;
uniform sampler2D u_previewSceneTexture;
uniform float u_previewSize;

varying vec2 v_uv;

const float previewWindowBorder = 0.02;
const float previewWindowBorderColor = 0.8;

void main()
{
    if (v_uv.s <= u_previewSize && v_uv.t > (1.0 - u_previewSize)) {
        float preview_u = v_uv.s * (1.0 / u_previewSize);
        float preview_v = (v_uv.t - (1.0 - u_previewSize)) * (1.0 / u_previewSize);
        if (preview_u <= previewWindowBorder ||
                preview_u >= (1.0 - previewWindowBorder) ||
                preview_v <= previewWindowBorder ||
                preview_v >= (1.0 - previewWindowBorder)) {
            gl_FragColor = vec4(vec3(previewWindowBorderColor), 1.0 );
        } else {
            gl_FragColor = texture2D(u_previewSceneTexture, vec2(preview_u, preview_v));
        }
    } else {
        gl_FragColor = texture2D(u_sceneTexture, v_uv);
    }
    // gl_FragColor = texture2D(u_sceneTexture, v_uv);
}

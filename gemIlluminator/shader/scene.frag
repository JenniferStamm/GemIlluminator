#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D u_sceneTexture;
uniform sampler2D u_previewSceneTexture;
uniform float u_previewSize;

varying vec2 v_uv;

void main()
{
    if (v_uv.s <= u_previewSize && v_uv.t > (1.0 - u_previewSize)) {
        float preview_u = v_uv.s * (1.0 / u_previewSize);
        float preview_v = (v_uv.t - (1.0 - u_previewSize)) * (1.0 / u_previewSize);
        gl_FragColor = texture2D(u_previewSceneTexture, vec2(preview_u, preview_v));
    } else {
        gl_FragColor = texture2D(u_sceneTexture, v_uv);
    }
    // gl_FragColor = texture2D(u_sceneTexture, v_uv);
}

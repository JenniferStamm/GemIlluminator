#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D u_sceneTexture;
uniform sampler2D u_previewSceneTexture;

varying vec2 v_uv;

void main()
{
    if (v_uv.s <= 0.5 && v_uv.t > 0.5) {
        float preview_u = v_uv.s * 2;
        float preview_v = (v_uv.t - 0.5) * 2;
        gl_FragColor = texture2D(u_previewSceneTexture, vec2(preview_u, preview_v));
    } else {
        gl_FragColor = texture2D(u_sceneTexture, v_uv);
    }
    // gl_FragColor = texture2D(u_sceneTexture, v_uv);
}

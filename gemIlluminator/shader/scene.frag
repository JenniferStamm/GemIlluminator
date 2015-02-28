#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D u_sceneTexture;
uniform sampler2D u_previewSceneTexture;
uniform sampler2D u_lightRayTexture;
uniform float u_previewSize;

varying vec2 v_uv;

const float previewWindowBorder = 0.02;
const float previewWindowBorderColor = 0.8;
const float glowFactor = 1.0;

void main()
{
    /* Original Shader Code
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
    */

    /* Shader Code without Conditional Branching */
    float preview_u = v_uv.s / u_previewSize;
    float preview_v = (v_uv.t - 1.0) / u_previewSize + 1.0;
    vec4 previewColor = texture2D(u_previewSceneTexture, vec2(preview_u, preview_v));
    vec4 previewBorderColor = vec4(vec3(previewWindowBorderColor), 1.0 );

    vec4 sceneColor = texture2D(u_sceneTexture, v_uv);
    vec4 lightRayColor = texture2D(u_lightRayTexture, v_uv);
    sceneColor += lightRayColor * glowFactor * lightRayColor.a;

    float previewMix = ceil((
                step(preview_u, previewWindowBorder) +
                step(1.0 - previewWindowBorder, preview_u) +
                step(preview_v, previewWindowBorder) +
                step(1.0 - previewWindowBorder, preview_v)) / 4.0
                );
    previewColor = mix(previewColor, previewBorderColor, previewMix);

    float sceneMix = step(v_uv.s, u_previewSize) * step(1.0 - u_previewSize, v_uv.t);
    gl_FragColor = vec4(mix(sceneColor, previewColor, sceneMix).xyz, 1.0);
   // gl_FragColor = vec4(lightRayColor.a, 0.0, 0.0, 1.0);
}

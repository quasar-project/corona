#version 440

layout(location = 0) in vec2 v_texcoord;
layout(location = 1) in vec4 v_color;

layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
    mat4 mvp;
    float opacity;
    // Windows HDR: set to SDR_white_level_in_nits / 80
    // macOS/iOS EDR: set to 1.0
    // No HDR: set to 0.0, will do linear to sRGB at the end then.
    float hdrWhiteLevelMult;
};

layout(binding = 1) uniform sampler2D tex;

void main()
{
    vec4 c = v_color * texture(tex, v_texcoord);
    c.a *= opacity;
    c.rgb *= c.a; // premultiplied alpha
    fragColor = c;
}
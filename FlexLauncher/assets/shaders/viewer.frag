#version 460 core

in vec2 texCoord;
out vec4 finalColor;

const uint HAS_BASE_COLOR_TEXTURE = 1;

layout(location = 0) uniform sampler2D albedoTexture;
layout(location = 1, std140) uniform MaterialUniforms {
    vec4 baseColorFactor;
    float alphaCutoff;
    uint flags;
} material;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    vec4 color = material.baseColorFactor;
    if ((material.flags & HAS_BASE_COLOR_TEXTURE) == HAS_BASE_COLOR_TEXTURE) {
        color *= texture(albedoTexture, texCoord);
    }
    float factor = (rand(gl_FragCoord.xy) - 0.5) / 8;
    if (color.a < material.alphaCutoff + factor)
        discard;
    finalColor = color;
}
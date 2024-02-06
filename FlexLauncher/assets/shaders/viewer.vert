#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 inTexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

out vec2 texCoord;

void main() {
    gl_Position = viewProjectionMatrix * modelMatrix * vec4(position, 1.0);
    texCoord = inTexCoord;
}
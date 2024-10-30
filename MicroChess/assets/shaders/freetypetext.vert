#version 460 core

// Input vertex data
layout(location = 0) in vec4 vertex; // (pos.x, pos.y, tex.s, tex.t)

// Uniforms
//uniform mat4 u_model;       // converts local space to world space
//uniform mat4 u_view;        // converts world space to view space
//uniform mat4 u_projection;  // converts view space to clip space
//uniform mat4 u_projection_view;
uniform mat4 projection;

// Output data
out vec2 TexCoords;

void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
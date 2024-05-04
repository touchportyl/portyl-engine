#version 460 core

// Input vertex data
layout (location = 0) in vec3 m_position;
layout (location = 1) in vec3 m_color;
layout (location = 2) in vec2 m_tex_coord;

// Uniforms
uniform mat4 u_model;       // converts local space to world space
uniform mat4 u_view;        // converts world space to view space
uniform mat4 u_projection;  // converts view space to clip space

// Output data
out vec3 color;
out vec2 tex_coord;

void main()
{
  // multiplication is right to left
  gl_Position = u_projection * u_view * u_model * vec4(m_position, 1.0);
  color = m_color;

  // flip textures vertically
  tex_coord = vec2(m_tex_coord.x, 1.0 - m_tex_coord.y);
}

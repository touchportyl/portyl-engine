#version 460 core

// Input vertex data
layout (location = 0) in vec3 m_position;
layout (location = 1) in vec3 m_color;
layout (location = 2) in vec2 m_tex_coord;
layout (location = 3) in vec3 m_normal;
layout (location = 4) in vec3 m_tangent;
layout (location = 5) in vec3 m_bitangent;

// Uniforms
uniform mat4 u_model;       // converts local space to world space
uniform mat4 u_view;        // converts world space to view space
uniform mat4 u_projection;  // converts view space to clip space

// Output data
out vec3 fragment_position;
out vec3 color;
out vec2 tex_coord;
out vec3 normal;

void main()
{
  // multiplication is right to left
  gl_Position = u_projection * u_view * u_model * vec4(m_position, 1.0);

  // calculate the fragment position in world space
  fragment_position = vec3(u_model * vec4(m_position, 1.0));

  // pass through data
  color = m_color;
  normal = m_normal;
  tex_coord = m_tex_coord;
  // flip textures vertically
  //tex_coord = vec2(m_tex_coord.x, 1.0 - m_tex_coord.y);
}

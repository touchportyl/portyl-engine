#version 460 core

// Input vertex data
layout (location = 0) in vec3 m_position;
layout (location = 1) in vec2 m_tex_coord;

// Instance attributes (unique per instance)
layout(location = 2) in mat4 m_Model;     // Model matrix for each instance
layout(location = 6) in vec3 m_Color;     // Color for each instance
layout(location = 7) in vec3 m_ColorAdd;  // Color add (extra tint)
layout(location = 8) in vec3 m_ColorMul;  // Color multiply (extra tint)

//uniform mat4 u_view;        // converts world space to view space
//uniform mat4 u_projection;  // converts view space to clip space
uniform mat4 u_projection_view;

// Output data
out vec2 tex_coord;
out vec3 instanceColor; // Pass instance color to fragment shader
out vec3 colorAdd;      // Color addition
out vec3 colorMul;      // Color multiplication

void main()
{
  // multiplication is right to left
  gl_Position = u_projection_view * m_Model * vec4(m_position, 1.0);

  // data passthrough
  tex_coord = m_tex_coord;
  instanceColor = m_Color;
  colorAdd = m_ColorAdd;
  colorMul = m_ColorMul;
}

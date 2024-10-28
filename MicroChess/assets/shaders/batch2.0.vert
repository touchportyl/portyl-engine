#version 460 core

// Input vertex data
layout (location = 0) in vec3 m_position;
layout (location = 1) in vec2 m_tex_coord;

//layout(std140, binding = 0) buffer m_InstanceTransforms 
//{
//    mat4 m_instanceTransforms[]; // Array of transformations for each instance
//};
// SSBO for instance data
 //   struct InstanceData {
 //       mat4 transform;            // Transformation matrix
 //       vec3 color_to_add;         // Color addition effect
 //       float padding1;
 //       vec3 color_to_multiply;    // Color multiplication effect
 //       float padding2;            
 //   };

layout(std430, binding = 0) buffer InstanceDataBuffer {
    mat4 transformation[];  // Array of instance data
};
// SSBO for colors (binding point 1)
layout(std430, binding = 1) buffer ColorBuffer {
    vec3 u_Color[];
};

// Uniforms
//uniform mat4 u_model;       // converts local space to world space
//uniform mat4 u_view;        // converts world space to view space
//uniform mat4 u_projection;  // converts view space to clip space
uniform mat4 u_projection_view;

// Output data
out vec2 tex_coord;
out vec3 u_color_to_add;

void main()
{

  // multiplication is right to left
  gl_Position = u_projection_view * transformation[gl_InstanceID] * vec4(m_position, 1.0);

  // data passthrough
  tex_coord = m_tex_coord;
  u_color_to_add = u_Color[gl_InstanceID];
}

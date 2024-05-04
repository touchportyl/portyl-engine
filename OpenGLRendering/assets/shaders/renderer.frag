#version 460 core

out vec4 fragment_color;
  
in vec3 color;
in vec2 tex_coord;

uniform sampler2D texture0;

void main()
{
    fragment_color = texture(texture0, tex_coord);
}
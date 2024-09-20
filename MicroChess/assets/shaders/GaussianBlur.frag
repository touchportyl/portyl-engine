#version 330 core
out vec4 FragColor;
  
in vec2 tex_coord;

uniform sampler2D screenTexture;
uniform bool horizontal;

// How far from the center to take samples from the fragment you are currently on
const int radius = 6;
// Keep it between 1.0f and 2.0f (the higher this is the further the blur reaches)
float spreadBlur = 2.0f;
float weights[radius];

void main()
{             

    FragColor = vec4(1.0f,0.0f,0.0f,1.0f);
}
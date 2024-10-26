#version 460 core

out vec4 fragment_color;

in vec2 tex_coord;
in vec3 instanceColor;
in vec3 colorAdd;     
in vec3 colorMul;     

// texture
uniform sampler2D u_texture;
uniform bool u_use_texture;

void main()
{
	vec4 texColor = u_use_texture ? texture(u_texture, tex_coord) : vec4(1.0);
	vec3 finalColor = texColor.rgb * instanceColor * colorMul + colorAdd;
	fragment_color = vec4(finalColor, texColor.a);
}

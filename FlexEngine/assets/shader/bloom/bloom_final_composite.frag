#version 330 core

out vec4 FragColor;
in vec2 tex_coord;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;
uniform float opacity;  // Controls bloom intensity

void main()
{
    // Fetch base color from the screen texture
    vec3 fragment = texture(screenTexture, tex_coord).rgb;

    // Fetch bloom color
    vec3 bloom = texture(bloomTexture, tex_coord).rgb;

    // Perform overlay blending with opacity control
    vec3 blended = mix(fragment, fragment + bloom, opacity); //might need to change

    // Output the final fragment color
    FragColor = vec4(blended, 1.0);  // Full alpha
}
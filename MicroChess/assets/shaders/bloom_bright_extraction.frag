#version 460 core

// First part of bloom -> Brightness Extraction

// Input texture coordinates from the vertex shader
in vec2 tex_coord;

// Output the resulting color after brightness extraction
out vec4 fragment_color;

// Uniforms for texture and brightness threshold
uniform sampler2D u_texture;          // Scene or diffuse texture
uniform float uniform_Threshold;      // Brightness threshold

uniform vec3 u_color;                 // Color if no texture is used
uniform bool u_use_texture;           // Flag to indicate whether to use texture or color

void main()
{
    vec3 diffuse;
    float alpha = 1.0;

    // Sample the texture or use the provided color
    if (u_use_texture) {
        vec4 texel = texture(u_texture, tex_coord);
        diffuse = texel.rgb;
        alpha = texel.a;
    } else {
        diffuse = u_color;
    }

    // Compute the luminance of the texture color
    vec3 luminances = vec3(0.2126, 0.7152, 0.0722);
    float luminance = dot(luminances, diffuse);  // Calculate the luminance of the pixel

    // Adjust the brightness using the uniform threshold
    luminance = max(0.0, luminance - uniform_Threshold);

    // Apply brightness extraction logic
    diffuse *= sign(luminance);  // If luminance is below the threshold, diffuse will be black
    fragment_color = vec4(diffuse, alpha);  // Keep alpha from texture or set to 1.0 for solid color
}
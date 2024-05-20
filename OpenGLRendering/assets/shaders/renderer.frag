#version 460 core

out vec4 fragment_color;

in vec3 fragment_position;
in vec3 color;
in vec2 tex_coord;
in vec3 normal; // normalized

//uniform vec3 u_view_position;

// material
// phong shading
uniform sampler2D u_material_diffuse0;
uniform sampler2D u_material_specular0;
uniform float u_material_shininess;

// directional light
uniform vec3 u_directional_light_direction;
uniform vec3 u_directional_light_ambient;
uniform vec3 u_directional_light_diffuse;
uniform vec3 u_directional_light_specular;

// point light
uniform vec3 u_point_light_position0;
uniform vec3 u_point_light_ambient0;
uniform vec3 u_point_light_diffuse0;
uniform vec3 u_point_light_specular0;

uniform vec3 u_point_light_position1;
uniform vec3 u_point_light_ambient1;
uniform vec3 u_point_light_diffuse1;
uniform vec3 u_point_light_specular1;

void main()
{
  //fragment_color = vec4(1.0, 0.0, 1.0, 1.0);

  //vec3 diffuse = texture(u_texture_diffuse0, tex_coord).rgb;
  //vec3 result = diffuse;
  //fragment_color = vec4(result, 1.0);

  // ambient
  vec3 ambient = min(u_directional_light_ambient + u_point_light_ambient0 + u_point_light_ambient1, 1.0) * texture(u_material_diffuse0, tex_coord).rgb;
  
  // diffuse
  float directional_light_diff = max(dot(normal, u_directional_light_direction), 0.0);

  vec3 point_light_direction0 = normalize(u_point_light_position0 - fragment_position);
  float point_light_diff0 = max(dot(normal, point_light_direction0), 0.0);

  vec3 point_light_direction1 = normalize(u_point_light_position1 - fragment_position);
  float point_light_diff1 = max(dot(normal, point_light_direction1), 0.0);

  vec3 diffuse = min(
    (u_directional_light_diffuse * directional_light_diff) +
    (u_point_light_diffuse0 * point_light_diff0) +
    (u_point_light_diffuse1 * point_light_diff1),
    1.0
  ) * texture(u_material_diffuse0, tex_coord).rgb;
  
  //// specular
  //vec3 view_direction = normalize(u_view_position - fragment_position);
  //vec3 reflection_direction = reflect(-light_direction, norm);
  //float spec = pow(max(dot(view_direction, reflection_direction), 0.0), u_material.shininess);
  //vec3 specular = light.specular * spec * texture(u_material.specular, tex_coord).rgb;
  
  vec3 result = ambient + diffuse;// + specular;
  fragment_color = vec4(result, 1.0);
}

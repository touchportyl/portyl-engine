#version 460 core

out vec4 fragment_color;

//struct Material
//{
//  sampler2D diffuse;
//  sampler2D specular;
//  float shininess;
//};

//struct Light
//{
//  vec3 position;
//
//  vec3 ambient;
//  vec3 diffuse;
//  vec3 specular;
//};

in vec3 fragment_position;
in vec3 color;
in vec2 tex_coord;
in vec3 normal;

//uniform vec3 u_view_position;
//uniform Material u_material;
//uniform Light light;

// material
uniform sampler2D u_material_diffuse0;
uniform sampler2D u_material_specular0;
uniform float u_material_shininess;

// directional light
uniform vec3 u_light_direction;
uniform vec3 u_light_ambient;
uniform vec3 u_light_diffuse;
uniform vec3 u_light_specular;

void main()
{
  //fragment_color = vec4(1.0, 0.0, 1.0, 1.0);

  //vec3 diffuse = texture(u_texture_diffuse0, tex_coord).rgb;
  //vec3 result = diffuse;
  //fragment_color = vec4(result, 1.0);

  // ambient
  vec3 ambient = u_light_ambient * texture(u_material_diffuse0, tex_coord).rgb;
  
  // diffuse
  vec3 norm = normalize(normal);
  //vec3 light_direction = normalize(light.position - fragment_position);
  vec3 light_direction = u_light_direction;
  float diff = max(dot(norm, light_direction), 0.0);
  vec3 diffuse = u_light_diffuse * diff * texture(u_material_diffuse0, tex_coord).rgb;
  
  //// specular
  //vec3 view_direction = normalize(u_view_position - fragment_position);
  //vec3 reflection_direction = reflect(-light_direction, norm);
  //float spec = pow(max(dot(view_direction, reflection_direction), 0.0), u_material.shininess);
  //vec3 specular = light.specular * spec * texture(u_material.specular, tex_coord).rgb;
  
  vec3 result = ambient + diffuse;// + specular;
  fragment_color = vec4(result, 1.0);
}

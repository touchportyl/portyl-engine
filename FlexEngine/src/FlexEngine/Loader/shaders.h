#pragma once

#include <string>

#include <glad/glad.h>

namespace FlexEngine
{

  /// <summary>
  /// Helper class for building and compiling shaders using glad
  /// </summary>
  class Shader
  {
  public:
    Shader();
    ~Shader();

    /// <summary>
    /// Destroy the shader program and its components
    /// </summary>
    void Destroy();

    /// <summary>
    /// Set the base path for shader files
    /// </summary>
    /// <param name="base_path">The base path for shader files "base/file/path"</param>
    Shader* SetBasePath(const std::string& base_path);

    /// <summary>
    /// Read and compile a vertex shader for linking
    /// </summary>
    /// <param name="path_to_vertex_shader">The path to the vertex shader file "path/to/vertex_shader"</param>
    Shader* CreateVertexShader(const std::string& path_to_vertex_shader);

    /// <summary>
    /// Read and compile a fragment shader for linking
    /// </summary>
    /// <param name="path_to_fragment_shader">The path to the fragment shader file "path/to/fragment_shader"</param>
    Shader* CreateFragmentShader(const std::string& path_to_fragment_shader);

    /// <summary>
    /// Link the vertex and fragment shaders into a shader program
    /// </summary>
    void Link();

    /// <summary>
    /// Use the shader program with glUseProgram
    /// </summary>
    void Use() const { glUseProgram(m_shader_program); }

    /// <returns>The shader program</returns>
    unsigned int Get() const { return m_shader_program; }

  private:
    std::string m_base_path{};
    unsigned int m_shader_program{};
    unsigned int m_vertex_shader{};
    unsigned int m_fragment_shader{};
  };

}
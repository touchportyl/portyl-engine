#pragma once

#include <cassert>
#include <iostream>
#include <fstream>
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
    Shader() {}
    ~Shader() {}

    /// <summary>
    /// Destroy the shader program and its components
    /// </summary>
    void Destroy()
    {
      if (m_vertex_shader != 0) glDeleteShader(m_vertex_shader);
      if (m_fragment_shader != 0) glDeleteShader(m_fragment_shader);
      if (m_shader_program != 0) glDeleteProgram(m_shader_program);
    }

    /// <summary>
    /// Set the base path for shader files
    /// </summary>
    /// <param name="base_path">The base path for shader files "base/file/path"</param>
    Shader* SetBasePath(const std::string& base_path = "")
    {
      m_base_path = base_path;
      return this;
    }

    /// <summary>
    /// Read and compile a vertex shader for linking
    /// </summary>
    /// <param name="path_to_vertex_shader">The path to the vertex shader file "path/to/vertex_shader"</param>
    Shader* CreateVertexShader(const std::string& path_to_vertex_shader)
    {
      // chain error handling
      if (this == nullptr) return nullptr;

      // warning if vertex shader already exists
      // handled by overriding the old shader
      if (m_vertex_shader != 0) {
        std::cout << "WARNING::SHADER::VERTEX::ALREADY_EXISTS" << std::endl;
        return nullptr;
      }

      // read vertex shader file
      std::string vertex_shader_source;
      std::ifstream ifs(m_base_path + '/' + path_to_vertex_shader);
      try
      {
        vertex_shader_source.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
      }
      catch (const std::ifstream::failure& e)
      {
        std::cout << "ERROR::SHADER::VERTEX::FILE_NOT_SUCCESFULLY_READ\n" << e.what() << std::endl;
        return nullptr;
      }

      // create vertex shader
      m_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
      const char* vertex_shader_source_cstr = vertex_shader_source.c_str();
      glShaderSource(m_vertex_shader, 1, &vertex_shader_source_cstr, NULL);
      glCompileShader(m_vertex_shader);

      // check for vertex shader compile errors
      int success;
      char infoLog[512];
      glGetShaderiv(m_vertex_shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(m_vertex_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return nullptr;
      }

      return this;
    }

    /// <summary>
    /// Read and compile a fragment shader for linking
    /// </summary>
    /// <param name="path_to_fragment_shader">The path to the fragment shader file "path/to/fragment_shader"</param>
    Shader* CreateFragmentShader(const std::string& path_to_fragment_shader)
    {
      // chain error handling
      if (this == nullptr) return nullptr;

      // warning if fragment shader already exists
      // handled by overriding the old shader
      if (m_fragment_shader != 0) {
        std::cout << "WARNING::SHADER::FRAGMENT::ALREADY_EXISTS" << std::endl;
        return nullptr;
      }

      // read fragment shader file
      std::string fragment_shader_source;
      std::ifstream ifs(m_base_path + '/' + path_to_fragment_shader);
      try
      {
        fragment_shader_source.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
      }
      catch (const std::ifstream::failure& e)
      {
        std::cout << "ERROR::SHADER::FRAGMENT::FILE_NOT_SUCCESFULLY_READ\n" << e.what() << std::endl;
        return nullptr;
      }

      // create fragment shader
      m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
      const char* fragment_shader_source_cstr = fragment_shader_source.c_str();
      glShaderSource(m_fragment_shader, 1, &fragment_shader_source_cstr, NULL);
      glCompileShader(m_fragment_shader);

      // check for fragment shader compile errors
      int success;
      char infoLog[512];
      glGetShaderiv(m_fragment_shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(m_fragment_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return nullptr;
      }

      return this;
    }

    /// <summary>
    /// Link the vertex and fragment shaders into a shader program
    /// </summary>
    void Link()
    {
      if (this == nullptr || // chain error handling
        m_vertex_shader == 0 || m_fragment_shader == 0) // check if shaders are compiled
      {
        std::cout << "ERROR::SHADER::LINKER::SHADERS_NOT_COMPILED" << std::endl;
        assert(false);
      }

      // create shader program
      m_shader_program = glCreateProgram();

      glAttachShader(m_shader_program, m_vertex_shader);
      glAttachShader(m_shader_program, m_fragment_shader);
      glLinkProgram(m_shader_program);

      // check for linking errors
      int success;
      char infoLog[512];
      glGetProgramiv(m_shader_program, GL_LINK_STATUS, &success);
      if (!success) {
        glDetachShader(m_shader_program, m_vertex_shader);
        glDetachShader(m_shader_program, m_fragment_shader);
        glGetProgramInfoLog(m_shader_program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        assert(false);
      }

      // delete shaders
      glDeleteShader(m_vertex_shader);
      glDeleteShader(m_fragment_shader);
    }

    /// <summary>
    /// Use the shader program with glUseProgram
    /// </summary>
    void Use() const
    {
      glUseProgram(m_shader_program);
    }

    /// <returns>The shader program</returns>
    unsigned int Get() const
    {
      return m_shader_program;
    }

  private:
    std::string m_base_path{};
    unsigned int m_shader_program{};
    unsigned int m_vertex_shader{};
    unsigned int m_fragment_shader{};
  };

}
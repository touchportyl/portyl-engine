#include "pch.h"

#include "opengltextures.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace FlexEngine
{

  #pragma region Static Internal Functions

  // Create a default texture
  // This is the legendary Valve purple and black checkered texture
  static void Internal_CreateDefaultTexture(unsigned char** texture_data, int width, int height)
  {
    *texture_data = new unsigned char[width * height * 4];
    if (!(*texture_data))
    {
      Log::Error("Could not allocate memory for default texture data!");
      return;
    }

    for (int y = 0; y < height; ++y)
    {
      for (int x = 0; x < width; ++x)
      {
        unsigned char color = ( ((x / 32) + (y / 32)) % 2 == 0 ) ? 255 : 0;

        int index = (y * width + x) * 4;
        (*texture_data)[index + 0] = color; // Red
        (*texture_data)[index + 1] = 0;     // Green
        (*texture_data)[index + 2] = color; // Blue
        (*texture_data)[index + 3] = 255;   // Alpha
      }
    }
  }
  
  static void Internal_LoadDefaultTexture(unsigned char** texture_data, unsigned int* out_texture, int width, int height)
  {
    // create default texture
    glGenTextures(1, out_texture);
    glBindTexture(GL_TEXTURE_2D, *out_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    Internal_CreateDefaultTexture(texture_data, width, height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
  }
  
  static bool Internal_LoadTextureFromFile(const char* filename, unsigned char** texture_data, unsigned int* out_texture, int* out_width, int* out_height)
  {
    // The usual image format has the origin at the top-left corner.
    // OpenGL expects the origin at the bottom-left corner.
    // We handle this in the vertex shader by flipping the y-coordinate.
    // The reason is so that ImGui can display the texture correctly because
    // it does its own texture flipping.
    //stbi_set_flip_vertically_on_load(true);

    // Load from file
    unsigned char* image_data = stbi_load(filename, out_width, out_height, NULL, 4);
    if (!image_data)
    {
      Log::Warning(std::string("Could not load texture file: ") + filename);
      return false;
    }

    // Save the image data
    std::size_t texture_size = (*out_width) * (*out_height) * 4;
    *texture_data = new unsigned char[texture_size];
    if (!(*texture_data))
    {
      Log::Error(std::string("Could not allocate memory for texture data: ") + filename);
      stbi_image_free(image_data);
      return false;
    }
    memcpy(*texture_data, image_data, texture_size);

    // Create a OpenGL texture identifier
    glGenTextures(1, out_texture);
    glBindTexture(GL_TEXTURE_2D, *out_texture);

    // Setup filtering parameters for display
    //glGenerateMipmap(GL_TEXTURE_2D);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into opengl texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *out_width, *out_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    return true;
  }

  #pragma endregion
  
  namespace Asset
  {

    // static member initialization
    const Texture Texture::Null = Default;
    const Texture Texture::None = Default;
    const Texture Texture::Default = Texture();

    Texture::~Texture()
    {
      Unload();
    }

    #pragma region Binding functions for OpenGL

    void Texture::Bind(unsigned int slot) const
    {
      glActiveTexture(GL_TEXTURE0 + slot);
      glBindTexture(GL_TEXTURE_2D, m_texture);
    }

    void Texture::Unbind() const
    {
      glBindTexture(GL_TEXTURE_2D, 0);
    }

    #pragma endregion

    #pragma region Texture Management Functions

    void Texture::Load()
    {
      // always unload the texture before loading
      Unload();

      // set the texture to be the default texture
      m_width = m_height = 64;
      Internal_LoadDefaultTexture(&m_texture_data, &m_texture, m_width, m_height);
    }

    void Texture::Load(const Path& path_to_texture)
    {
      // always unload the texture before loading
      Unload();

      bool success = Internal_LoadTextureFromFile(path_to_texture.string().c_str(), &m_texture_data, &m_texture, &m_width, &m_height);
      // if no texture is loaded, bind the default texture
      if (!success || !m_texture || !m_width || !m_height)
      {
        Load();
      }
    }

    void Texture::Unload()
    {
      if (m_texture_data)
      {
        delete[] m_texture_data;
        m_texture_data = nullptr;
      }

      if (m_texture)
      {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
      }

      m_width = m_height = 0;
    }

    #pragma endregion

  }

}

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
  static void CreateDefaultTexture(GLubyte* data, int width, int height)
  {
    for (int y = 0; y < height; ++y)
    {
      for (int x = 0; x < width; ++x)
      {
        GLubyte color = ((x / 32) + (y / 32)) % 2 == 0 ? 255 : 0;
        data[(y * width + x) * 4 + 0] = color; // Red
        data[(y * width + x) * 4 + 1] = 0;     // Green
        data[(y * width + x) * 4 + 2] = color; // Blue
        data[(y * width + x) * 4 + 3] = 255;   // Alpha
      }
    }
  }
  
  static void LoadDefaultTexture(unsigned int* out_texture, int* out_width, int* out_height)
  {
    // create default texture
    glGenTextures(1, out_texture);
    glBindTexture(GL_TEXTURE_2D, *out_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    GLubyte* data = new GLubyte[64 * 64 * 4];
    CreateDefaultTexture(data, 64, 64);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    delete[] data;

    glBindTexture(GL_TEXTURE_2D, 0);

    *out_width = 64;
    *out_height = 64;
  }
  
  static bool LoadTextureFromFile(const char* filename, unsigned int* out_texture, int* out_width, int* out_height)
  {
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
    {
      Log::Warning(std::string("Could not load texture file ") + filename);
      return false;
    }

    // Create a OpenGL texture identifier
    unsigned int image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    //glGenerateMipmap(GL_TEXTURE_2D);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
  }

  #pragma endregion
  
  namespace Asset
  {

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
      unsigned int default_texture;
      int default_width;
      int default_height;
      LoadDefaultTexture(&default_texture, &default_width, &default_height);

      // set the texture to be the default texture
      m_texture = default_texture;
      m_width = default_width;
      m_height = default_height;
    }

    void Texture::Load(const Path& path_to_texture)
    {
      bool success = LoadTextureFromFile(path_to_texture.string().c_str(), &m_texture, &m_width, &m_height);
      // if no texture is loaded, bind the default texture
      if (!success || m_texture == 0 || m_width == 0 || m_height == 0)
      {
        Load();
      }
    }

    void Texture::Unload() const
    {
      if (m_texture != 0)
      {
        glDeleteTextures(1, &m_texture);
      }
    }

    #pragma endregion

  }

}

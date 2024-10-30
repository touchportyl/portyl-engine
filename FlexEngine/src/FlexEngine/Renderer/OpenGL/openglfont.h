#pragma once

#include "flx_api.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include "Wrapper/file.h"
#include "Renderer/OpenGL/openglshader.h"
#include <glad/glad.h>
#include <string>

// Helper macro to display a texture for ImGui using the ImGui::Image function
// Example usage: ImGui::Image(IMGUI_IMAGE(texture_variable));
#define IMGUI_IMAGE(TEXTURE) TEXTURE.GetTextureImGui(), ImVec2(TEXTURE.GetWidthF(), TEXTURE.GetHeightF())

namespace FlexEngine
{
  namespace Asset
  {

    struct __FLX_API Glyph
    {
        GLuint textureID;   // Texture ID
        Vector2 size;    // Size of glyph
        Vector2 bearing; // Offset from baseline to left/top of glyph
        GLuint advance;     // Horizontal offset to advance to next glyph
    };

    class __FLX_API Font
    {
      // The texture data is stored as an array of unsigned chars.
      // In OpenGL, the texture data is stored as RGBA.
      //unsigned char* m_font_data = nullptr;
      //unsigned int m_font = 0;
      //int m_width = 0;
      //int m_height = 0;

      // Static FreeType library and face for all Font instances
      static FT_Library s_library;
      static GLuint s_facesCount;
      FT_Face s_face;
      std::map<char, Glyph> m_glyphs;

    public:
      // Remove default constructor
      Font() = delete;

      // Explicitly require a key to initialize the font
      explicit Font(std::string const& key);

      // Copy constructor and copy assignment 
      //Font(const Font& other);
      //Font& operator=(const Font& other);

      // For creating textures from memory
      // Useful for creating textures from procedural generation or loading
      // embedded textures.
      //Font(unsigned char* texture_data, int width, int height = 0);

      //~Font();

      // Common texture names.
      // They are all represented by the default purple-black checkerboard texture.

      //static const Font Null;
      //static const Font None;
      //static Font Default();

      #pragma region Font Management Functions

      // Load a texture from a path
      void Load(const Path& path_to_texture);

      void Unload();

      void SetupLib();
      void LoadGlyphs();

      #pragma endregion

      #pragma region Binding functions for OpenGL

      void Bind(const Shader& shader, const char* name = "u_texture_diffuse", unsigned int texture_unit = 0) const;
      void Unbind() const;

      #pragma endregion

      #pragma region Operator Overloads

      //operator unsigned int() const { return m_font; }
      //operator unsigned char*() const { return m_font_data; }
      //operator bool() const { return m_font != 0; }

      //friend bool operator==(const Font& lhs, const Font& rhs) { return lhs.GetFont() == rhs.GetFont(); }
      //friend bool operator!=(const Font& lhs, const Font& rhs) { return lhs.GetFont() != rhs.GetFont(); }

      #pragma endregion

      // getter functions

      //unsigned int    GetFont()      const { return m_font; }
      //unsigned char*  GetFontData()  const { return m_font_data; }
      //int             GetWidth()        const { return m_width; }
      //int             GetHeight()       const { return m_height; }
      /*float           GetWidthF()       const { return static_cast<float>(m_width); }
      float           GetHeightF()      const { return static_cast<float>(m_height); }*/
      // Helper function for ImGui
      //void*           GetFontImGui() const { return (void*)(intptr_t)m_font; }

    };

  }
}

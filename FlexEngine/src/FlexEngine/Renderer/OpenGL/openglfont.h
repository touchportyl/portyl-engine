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
      // Static FreeType library and face for all Font instances
      static FT_Library s_library;
      static GLuint s_facesCount;
      FT_Face s_face;
      std::map<char, Glyph> m_glyphs;

      // Font properties
      int m_fontSize = 16;          // Default font size
      bool m_hintingEnabled = true; // Hinting flag
      bool m_kerningEnabled = true; // Kerning flag

    public:
      // Remove default constructor
      Font() = delete;

      // Explicitly require a key to initialize the font
      explicit Font(std::string const& key);

      #pragma region Font Management Functions

      // Load a texture from a path
      //void Load(const Path& path_to_texture);

      //Free all faces and library 
      void Unload();
      //Set up freetype library if not yet setup.
      void SetupLib();
      // Load all glyphs
      void LoadGlyphs();

      // Setters for font properties
      void SetFontSize(int size);
      void SetHinting(bool enabled);
      void SetKerning(bool enabled);

      #pragma endregion

      #pragma region Binding functions for OpenGL

      //void Bind(const Shader& shader, const char* name = "u_texture_diffuse", unsigned int texture_unit = 0) const;
      //void Unbind() const;

      #pragma endregion


      #pragma region Get Functions

      Glyph const& GetGlyph(char c) const;
      int GetFontSize() const { return m_fontSize; }
      bool IsHintingEnabled() const { return m_hintingEnabled; }
      bool IsKerningEnabled() const { return m_kerningEnabled; }
      // Helper function for ImGui
      //void*           GetFontImGui() const { return (void*)(intptr_t)m_font; }
      #pragma endregion
    };

  }
}

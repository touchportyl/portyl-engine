#include "pch.h"

#include "openglfont.h"



namespace FlexEngine
{
  namespace Asset
  {
    FT_Library Font::s_library{};
    GLuint Font::s_facesCount = 0;

    Font::Font(std::string const& key)
    {
        //Set up FreeType Lib
        SetupLib();

        // Load the font in font database
        if (FT_New_Face(s_library, Path::current("assets" + key).string().c_str(), 0, &s_face))
        {
            Log::Fatal("Could not load font, check font directory");
        }
        ++s_facesCount;

        //Load Glyphs
        FT_Set_Pixel_Sizes(s_face, 0, m_fontSize);
        LoadGlyphs();
    }

    #pragma region Font Management Functions

    void Font::Unload()
    {
        //Free the FreeType face object
        if (s_face)
        {
            Log::Debug("Unloaded font: " + std::string(s_face->family_name) + " " + std::string(s_face->style_name));
            FT_Done_Face(s_face);
            s_face = nullptr;
            --s_facesCount;
        }

        //Free the FreeType library object
        if (s_facesCount <= 0 && s_library)
        {
            FT_Done_FreeType(s_library);
            s_library = nullptr;
            Log::Info("All fonts unloaded.");
        }

        //Delete all textures in m_glyphs
        for (auto& pair : m_glyphs)
            glDeleteTextures(1, &pair.second.textureID);

        m_glyphs.clear();
    }

    void Font::SetupLib()
    {
        if (s_library == nullptr)
            if (FT_Init_FreeType(&s_library))
                Log::Fatal("Unable to initialize FreeType Library, check dll files.");
    }

    // Load all glyphs into OpenGL textures
    void Font::LoadGlyphs()
    {
        // Free any previously loaded glyphs
        for (auto& pair : m_glyphs)
        {
            glDeleteTextures(1, &pair.second.textureID);
        }
        m_glyphs.clear();

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

        for (unsigned char c = 0; c < 128; ++c)
        {
            int loadFlags = FT_LOAD_RENDER;
            if (!m_hintingEnabled)
                loadFlags |= FT_LOAD_NO_HINTING;

            // Load character glyph
            if (FT_Load_Char(s_face, c, loadFlags))
            {
                Log::Warning("Failed to load glyph for character: " + std::string(1, c));
                continue;
            }

            FT_GlyphSlot g = s_face->glyph;
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                g->bitmap.width,
                g->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                g->bitmap.buffer
            );

            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Store the glyph
            Glyph glyph = {
                texture,
                Vector2(g->bitmap.width, g->bitmap.rows),
                Vector2(g->bitmap_left, g->bitmap_top),
                static_cast<GLuint>(g->advance.x >> 6)
            };
            m_glyphs[c] = glyph;
        }

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Font::SetFontSize(int size)
    {
        if (size <= 0)
        {
            Log::Warning("Font size must be positive.");
            return;
        }
        m_fontSize = size;
        FT_Set_Pixel_Sizes(s_face, 0, m_fontSize);
        LoadGlyphs(); // Reload glyphs at the new size
    }

    void Font::SetHinting(bool enabled)
    {
        m_hintingEnabled = enabled;
        LoadGlyphs(); // Reload glyphs with updated hinting settings
    }

    void Font::SetKerning(bool enabled)
    {
        m_kerningEnabled = enabled;
    }

    #pragma endregion

    #pragma region Binding functions for OpenGL

    //void Font::Bind(const Shader& shader, const char* name, unsigned int font_unit) const
    //{
    //  glActiveTexture(GL_TEXTURE0 + font_unit);

    //  std::string font_name = name;
    //  font_name += std::to_string(font_unit);
    //  glUniform1i(glGetUniformLocation(shader.Get(), font_name.c_str()), font_unit);

    //  glBindTexture(GL_TEXTURE_2D, m_font);
    //}

    //void Font::Unbind() const
    //{
    //  glBindTexture(GL_TEXTURE_2D, 0);
    //}

    #pragma endregion

    #pragma region Getter functions
    const Glyph& Font::GetGlyph(char c) const 
    {
        // Return default glyph if character not found
        static const Glyph defaultGlyph = {};
        auto it = m_glyphs.find(c);
        return it != m_glyphs.end() ? it->second : defaultGlyph;
    }
    #pragma endregion
  }

}

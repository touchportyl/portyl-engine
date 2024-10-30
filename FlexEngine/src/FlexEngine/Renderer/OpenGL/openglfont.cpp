#include "pch.h"

#include "openglfont.h"



//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

namespace FlexEngine
{

  #pragma region Static Internal Functions

  static void Internal_LoadFontForOpenGL(unsigned int* out_font, unsigned char* font_data, int width, int height)
  {
    // Create a OpenGL texture identifier
    glGenTextures(1, out_font);
    glBindTexture(GL_TEXTURE_2D, *out_font);

    // Setup filtering parameters for display
    //glGenerateMipmap(GL_TEXTURE_2D);
    #if 0
    {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, glyph->bitmap.width, glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);

    }
    #endif

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into opengl texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, font_data);
  }

  // Create a default texture
  // This is the legendary purple and black checkered texture
  //static void Internal_CreateDefaultFont(unsigned char** out_font_data, int width, int height)
  //{
  //  *out_texture_data = new unsigned char[width * height * 4];
  //  if (!(*out_texture_data))
  //  {
  //    Log::Error("Could not allocate memory for default texture data!");
  //    return;
  //  }

  //  for (int y = 0; y < height; ++y)
  //  {
  //    for (int x = 0; x < width; ++x)
  //    {
  //      unsigned char color = ( ((x / 32) + (y / 32)) % 2 == 0 ) ? 255 : 0;

  //      int index = (y * width + x) * 4;
  //      (*out_texture_data)[index + 0] = color; // Red
  //      (*out_texture_data)[index + 1] = 0;     // Green
  //      (*out_texture_data)[index + 2] = color; // Blue
  //      (*out_texture_data)[index + 3] = 255;   // Alpha
  //    }
  //  }
  //}
  //
  //static void Internal_LoadDefaultFont(unsigned char** out_font_data, unsigned int* out_font, int width, int height)
  //{
  //  // create default texture
  //  Internal_CreateDefaultTexture(out_texture_data, width, height);

  //  // bind the default texture
  //  Internal_LoadTextureForOpenGL(out_texture, *out_texture_data, width, height);
  //}
  //

  //static bool Internal_LoadFontFromFile(const char* filename, unsigned char** out_font_data, unsigned int* out_font, int* out_width, int* out_height)
  //{
  //  // Load from file
  //  unsigned char* image_data = stbi_load(filename, out_width, out_height, NULL, 4);
  //  if (!image_data)
  //  {
  //    Log::Warning(std::string("Could not load font file: ") + filename);
  //    return false;
  //  }

  //  // Save the image data
  //  std::size_t font_size = (*out_width) * (*out_height) * 4;
  //  *out_font_data = new unsigned char[font_size];
  //  if (!(*out_font_data))
  //  {
  //    Log::Error(std::string("Could not allocate memory for font data: ") + filename);
  //    stbi_image_free(image_data);
  //    return false;
  //  }
  //  memcpy(*out_font_data, image_data, font_size);

  //  // Bind the texture
  //  Internal_LoadFontForOpenGL(out_font, *out_font_data, *out_width, *out_height);

  //  stbi_image_free(image_data);

  //  return true;
  //}

  /*static bool Internal_LoadFontFromMemory(const unsigned char* font_data, int size, unsigned char** out_font_data, unsigned int* out_font, int* out_width, int* out_height)
  {
    */  //// Decompress the font
      //int channels;
      //unsigned char* image_data = stbi_load_from_memory(texture_data, size, out_width, out_height, &channels, 4);
      //if (!image_data)
      //{
      //    Log::Error("Could not decompress texture data!");
      //    return false;
      //}

      //// Save the image data
      //std::size_t texture_size = (*out_width) * (*out_height) * 4;
      //*out_texture_data = new unsigned char[texture_size];
      //if (!(*out_texture_data))
      //{
      //    Log::Error("Could not allocate memory for texture data!");
      //    stbi_image_free(image_data);
      //    return false;
      //}
      //memcpy(*out_texture_data, image_data, texture_size);

      //// Bind the texture
      //Internal_LoadFontForOpenGL(out_texture, *out_texture_data, *out_width, *out_height);

      //stbi_image_free(image_data);

      //return true;
  //}

  #pragma endregion
  
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
        int font_size = 10; //Default
        FT_Set_Pixel_Sizes(s_face, 0, font_size); // Set font size
        LoadGlyphs(); //Seems correct lol
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
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

        // Load first 128 ASCII characters
        for (unsigned char c = 0; c < 128; ++c) 
        { 
            if (FT_Load_Char(s_face, c, FT_LOAD_RENDER)) 
            {
                Log::Warning("Failed to load glyph for character: " + std::string(1, c) + " - " + s_face->family_name+ s_face->style_name);
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

            // Store character for later use
            Glyph glyph = {
                texture,
                Vector2(g->bitmap.width, g->bitmap.rows), // Size of the glyph
                Vector2(g->bitmap_left, g->bitmap_top),   // Bearing (offset from baseline)
                static_cast<GLuint>(g->advance.x >> 6)    // Advance to next glyph (divided by 64)
            };
            m_glyphs[c] = glyph;
        }

        glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture
    }
    //Font& Font::operator=(const Font& other)
    //{
    //  // deep copy
    //  m_font = other.m_font;
    //  //m_width = other.m_width;
    //  //m_height = other.m_height;
    //  //std::size_t size = m_width * m_height * 4;
    //  if (!m_font)
    //  {
    //    m_font_data = new unsigned char[size];
    //    memcpy(m_font_data, other.m_font_data, size);
    //  }
    //  else
    //  {
    //    m_font_data = nullptr;
    //  }

    //  return *this;
    //}

    //Font::Font(unsigned char* font_data, int width, int height)
    //  : m_font_data(nullptr)/*, m_width(width), m_height(height)*/
    //{
      //// decompress the texture
      //if (height == 0)
      //{
      //  //bool success = Internal_LoadTextureFromMemory(texture_data, width, &m_texture_data, &m_texture, &m_width, &m_height);
      //  //// if no texture is loaded, bind the default texture
      //  //if (!success || !m_texture || !m_width || !m_height)
      //  //{
      //  //  Load();
      //  //}

      //  Log::Error("AssimpWrapper: Embedded textures are compressed, decompression is not supported.");
      //  Load();

      //  //Log::Debug("Loaded: " + std::to_string(m_texture));
      //}
      //// no decompression needed
      //else
      //{
      //  m_font_data = new unsigned char[width * height * 4];
      //  if (!m_font_data)
      //  {
      //    Log::Error("Could not allocate memory for font data!");
      //    return;
      //  }
      //  memcpy(m_font_data, font_data, width * height * 4);
      //  
      //  Internal_LoadTextureForOpenGL(&m_font, m_font_data, m_width, m_height);
      //}
    //}

    #pragma region Font Management Functions

    //void Font::Load()
    //{
    //  // always unload the texture before loading
    //  Unload();

    //  // set the texture to be the default texture
    //  m_width = m_height = 64;
    //  Internal_LoadDefaultTexture(&m_font_data, &m_font, m_width, m_height);
    //}

    void Font::Load(const Path& path_to_texture)
    {
      // always unload the texture before loading
      //Unload();

      // if font is unable to load 
      //if (!Internal_LoadFontFromFile(path_to_texture.string().c_str(), &m_font_data, &m_font, &m_width, &m_height))
      //{
      //    Log::Fatal("Invalid font file, pls check file.");
      //}
    }

    void Font::Unload()
    {
        std::string familyName = s_face ? s_face->family_name : "";
        std::string styleName = s_face ? s_face->style_name : "";
        if (styleName != "" && familyName != "") Log::Info("Unloaded font: " + familyName + " " + styleName);
        
        //Free the FreeType face object
        if (s_face)
        {
            FT_Done_Face(s_face);
            s_face = nullptr;
            --s_facesCount;
        }

        //Free the FreeType library object
        if (s_facesCount<=0 && s_library)
        {
            FT_Done_FreeType(s_library);
            s_library = nullptr;
        }

        //Delete all textures in m_glyphs
        for (auto& pair : m_glyphs)
        {
            glDeleteTextures(1, &pair.second.textureID);
        }
        m_glyphs.clear();
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

  }

}

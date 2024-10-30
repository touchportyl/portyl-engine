#pragma once

#include "FlexMath/vector4.h"
#include "flx_api.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "opengltexture.h"
#include "openglspriterenderer.h"
#include "../alphabetstorage.h"
#include <glad/glad.h>

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

#define DEFAULT_WIDTHS_COUNT 1024
#define DRAW_TEXT_MAX_STRLEN 2048
#define DEFAULT_FONT_SIZE    18

/** select one of the two values GL_UNSIGNED_INT/GL_UNSIGNED_SHORT according to the width of wchar_t */
#define GL_CHAR_SIZE_CODE (sizeof(wchar_t) == 4 ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT)

/** ... */
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

namespace FlexEngine 
{
    // Default constants and color definition
    //const GLubyte DEFAULT_TEXT_COLOR[] = { 255, 255, 255, 255 };   // Default text color
    const Vector4 DEFAULT_TEXT_COLOR = Vector4::One;

    struct __FLX_API F_Character 
    {
        GLuint textureID;   // Texture ID
        Vector2 size;    // Size of glyph
        Vector2 bearing; // Offset from baseline to left/top of glyph
        GLuint advance;     // Horizontal offset to advance to next glyph
    };


    class OpenGLFreeType 
    {
        FT_Library library;
        FT_Face face;

        std::vector<GLuint> textures;
        std::vector<unsigned int> chars_widths;

        GLuint vao, vbo;
        float density;
        unsigned int height;

        //void init(const std::string& file_name, const unsigned int* alphabet, unsigned int chars_count, int font_height, float density, int dpi, float line_spacing) noexcept;
        //bool setupFreeType(const std::string& file_name, int font_height, int dpi);
        //void initCharacterVAO();
        //unsigned int loadCharacter(unsigned int ch);
        //void renderCharacter(GLuint textureID, float x, float y, float width, float height);
    public:
        ////OpenGLFreeType(const std::string& file_name, int font_height, float density, int dpi, float line_spacing) noexcept;
        //OpenGLFreeType(const std::string& fontPath, unsigned int fontSize);
        //~OpenGLFreeType();
        static void Init();

        void drawText(const std::wstring& text, float x, float y, float scale);
    };
}
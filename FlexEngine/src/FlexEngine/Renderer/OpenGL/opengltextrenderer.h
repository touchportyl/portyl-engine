#pragma once

#include "FlexMath/vector4.h"
#include "flx_api.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "openglfont.h"
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

    struct __FLX_API Renderer2DText
    {
        std::string m_shader = R"(/shaders/texture)";
        std::string m_fonttype = R"()"; // Have a check if this is empty, send fatal error
        std::string m_words;
        Vector3 m_color = Vector3::Zero;
        Matrix4x4 m_transform = Matrix4x4::Identity;
        Vector3 m_pos = Vector3::Zero;
        Vector3 m_scale = Vector3::Zero;
    };

    class __FLX_API OpenGLTextRenderer
    {
        //static GLuint m_shaderProgram; // ID for the shader program
        static GLuint m_VAO, m_VBO; // Vertex Array Object and Vertex Buffer Object
        //float density;
        //unsigned int height;
        static void InitRenderData(); // Initializes VBO and VAO for rendering text
        static void RenderGlyph(const Asset::Glyph& glyph, Vector2 pos, float scale, const Vector3& color);

        //void init(const std::string& file_name, const unsigned int* alphabet, unsigned int chars_count, int font_height, float density, int dpi, float line_spacing) noexcept;
        //bool setupFreeType(const std::string& file_name, int font_height, int dpi);
        //void initCharacterVAO();
        //unsigned int loadCharacter(unsigned int ch);
        //void renderCharacter(GLuint textureID, float x, float y, float width, float height);
    public:
        static void Init();

        static void DrawText2D(const Renderer2DText& text);
    };
}
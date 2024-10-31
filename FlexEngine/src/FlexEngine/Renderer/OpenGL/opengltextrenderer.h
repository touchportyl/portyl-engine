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

namespace FlexEngine 
{
    struct __FLX_API Renderer2DText
    {
        enum __FLX_API AlignmentX
        {
            Alignment_Center = 0,      /*!< Center alignment */
            Alignment_Left = 1,        /*!< Left alignment */
            Alignment_Right = 2,        /*!< Right alignment */
        };

        enum __FLX_API AlignmentY
        {
            Alignment_Middle = 0,      /*!< Center alignment */
            Alignment_Top = 1,         /*!< Top alignment */
            Alignment_Bottom = 2,      /*!< Bottom alignment */
        };

        std::string m_shader = R"(/shaders/freetypetext)";
        std::string m_fonttype = R"()"; // Have a check if this is empty, send fatal error
        std::string m_words;
        Vector3 m_color = Vector3::Zero;
        Matrix4x4 m_transform = Matrix4x4::Identity;
        std::pair<AlignmentX, AlignmentY> m_alignment;
    };

    class __FLX_API OpenGLTextRenderer
    {
        static GLuint m_VAO, m_VBO; // Vertex Array Object and Vertex Buffer Object
        
        static float m_linespacing, m_letterspacing;

        static void InitRenderData(); // Initializes VBO and VAO for rendering text
        static void RenderGlyph(const Asset::Glyph& glyph, const Vector2& pen_pos, const Vector3& color);
        static int FindLineWidth(const Asset::Font& font, const std::string& text = "");

    public:
        static void Init();

        static void DrawText2D(const Renderer2DText& text);
    };
}
#include "opengltextrenderer.h"
#include "FlexEngine/AssetManager/assetmanager.h" // FLX_ASSET_GET
#include "FlexEngine/DataStructures/freequeue.h"

namespace FlexEngine
{
    GLuint OpenGLTextRenderer::m_VAO = 0;
    GLuint OpenGLTextRenderer::m_VBO = 0;
    float OpenGLTextRenderer::m_linespacing = 2.0f;
    float OpenGLTextRenderer::m_letterspacing = 5.0f;

    void OpenGLTextRenderer::Init()
    {
        InitRenderData();

    }
    void OpenGLTextRenderer::InitRenderData()
    {
        // Configure VAO/VBO for text quads
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW); // 6 vertices per quad

        // Configure vertex attributes
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        FreeQueue::Push(
          [=]()
        {
            glDeleteVertexArrays(1, &m_VAO);
            glDeleteBuffers(1, &m_VBO);
        }
        );
    }

    void OpenGLTextRenderer::DrawText2D(const Renderer2DText& text)
    {
        auto& asset_shader = FLX_ASSET_GET(Asset::Shader, text.m_shader);
        asset_shader.Use();

        if (text.m_fonttype.empty())
            Log::Fatal("Text Renderer: Unknown font type! Please check what you wrote!");
        if (text.m_words.empty())
            return;

        asset_shader.SetUniform_vec3("u_color", text.m_color);
        asset_shader.SetUniform_mat4("u_model", text.m_transform);
        static const Matrix4x4 view_matrix = Matrix4x4::LookAt(Vector3::Zero, Vector3::Forward, Vector3::Up);
        Matrix4x4 projection_view = Matrix4x4::Orthographic(0.0f, 1280, 750, 0.0f, -2.0f, 2.0f) * view_matrix;
        asset_shader.SetUniform_mat4("projection", projection_view);

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(m_VAO);
        auto& asset_font = FLX_ASSET_GET(Asset::Font, text.m_fonttype);

        // Determine initial X position based on horizontal alignment
        Vector3 pen_pos = Vector3::Zero;
        float lineWidth = 0.0f;
        float maxLineHeight = 0.0f;

        switch (static_cast<Renderer2DText::AlignmentX>(text.m_alignment.first))
        {
        case Renderer2DText::Alignment_Left:
            pen_pos.x = 0.0f;
            break;
        case Renderer2DText::Alignment_Center:
            pen_pos.x = FindLineWidth(asset_font, text.m_words) / 2.0f;
            break;
        case Renderer2DText::Alignment_Right:
            pen_pos.x = FindLineWidth(asset_font, text.m_words);
            break;
        }

        // Determine initial Y position based on vertical alignment (not working)
        //switch (static_cast<Renderer2DText::AlignmentY>(text.m_alignment.second))
        //{
        //case Renderer2DText::Alignment_Top:
        //    pen_pos.y = 0.0f;
        //    break;
        //case Renderer2DText::Alignment_Middle:
        //    pen_pos.y = maxHeight / 2.0f;
        //    break;
        //case Renderer2DText::Alignment_Bottom:
        //    pen_pos.y = maxHeight;
        //    break;
        //}

        // Iterate through all characters in the string
        for (char c : text.m_words)
        {
            // Get the glyph for the character
            const Asset::Glyph& glyph = asset_font.GetGlyph(c);
            maxLineHeight = std::max(maxLineHeight, glyph.size.y);

            // Check if adding this glyph exceeds max width, wrap to new line if necessary
            if (lineWidth + glyph.advance > DRAW_TEXT_MAX_STRLEN && c != ' ')
            {
                // Move to a new line
                pen_pos.y += maxLineHeight + m_linespacing; // Move down by glyph height and line spacing
                lineWidth = 0.0f;          // Reset line width for the new line

                // Adjust starting X position for new line based on alignment
                switch (static_cast<Renderer2DText::AlignmentX>(text.m_alignment.first))
                {
                case Renderer2DText::Alignment_Left:
                    pen_pos.x = 0.0f;
                    break;
                case Renderer2DText::Alignment_Center:
                    pen_pos.x = FindLineWidth(asset_font, text.m_words) / 2.0f;
                    break;
                case Renderer2DText::Alignment_Right:
                    pen_pos.x = FindLineWidth(asset_font, text.m_words);
                    break;
                }
            }

            // Render the glyph at the current position
            RenderGlyph(glyph, pen_pos, text.m_color);

            // Advance position for the next character
            pen_pos.x -= glyph.advance + m_letterspacing;  // Add spacing between characters
            lineWidth += glyph.advance + m_letterspacing;  // Track width for the current line
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    int OpenGLTextRenderer::FindLineWidth(const Asset::Font& font, const std::string& text)
    {
        int t_totallength = 0;
        for (char c : text)
        {
            // Get the glyph for the character
            const Asset::Glyph& glyph = font.GetGlyph(c);
            t_totallength += glyph.advance + m_letterspacing; // Add advance and spacing
        }
        return t_totallength > DRAW_TEXT_MAX_STRLEN ? DRAW_TEXT_MAX_STRLEN : t_totallength;
    }

    void OpenGLTextRenderer::RenderGlyph(const Asset::Glyph& glyph, const Vector2& pen_pos, const Vector3& color)
    {
        // Bind the texture for the glyph
        glBindTexture(GL_TEXTURE_2D, glyph.textureID);

        // Calculate position and size of the glyph
        float xpos = pen_pos.x - glyph.bearing.x;
        float ypos = pen_pos.y - (glyph.bearing.y - glyph.size.y); // Align glyph to baseline
        float w = -glyph.size.x;
        float h = -glyph.size.y;

        // Define the vertices for a quad (two triangles) for the glyph
        float vertices[6][4] = {
            { xpos,     ypos + h, 0.0f, 0.0f },
            { xpos,     ypos,     0.0f, 1.0f },
            { xpos + w, ypos,     1.0f, 1.0f },

            { xpos,     ypos + h, 0.0f, 0.0f },
            { xpos + w, ypos,     1.0f, 1.0f },
            { xpos + w, ypos + h, 1.0f, 0.0f }
        };

        // Update the VBO with the new vertices
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Render the quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}
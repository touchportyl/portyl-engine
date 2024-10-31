#include "opengltextrenderer.h"
#include "FlexEngine/AssetManager/assetmanager.h" // FLX_ASSET_GET
#include "FlexEngine/DataStructures/freequeue.h"

namespace FlexEngine
{
    GLuint OpenGLTextRenderer::m_VAO = 0;
    GLuint OpenGLTextRenderer::m_VBO = 0;

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
        // Apply Texture
        if (text.m_fonttype == "") Log::Fatal("Text Renderer: Unknown font type! Please check what u wrote!");

        asset_shader.SetUniform_vec3("u_color", text.m_color);
        asset_shader.SetUniform_mat4("u_model", text.m_transform);
        static const Matrix4x4 view_matrix = Matrix4x4::LookAt(Vector3::Zero, Vector3::Forward, Vector3::Up);
        Matrix4x4 projection_view = Matrix4x4::Orthographic(
          0.0f, 1280,
          750, 0.0f,
          -2.0f, 2.0f
        ) * view_matrix;
        asset_shader.SetUniform_mat4("projection", projection_view);

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(m_VAO);
        auto& asset_font = FLX_ASSET_GET(Asset::Font, text.m_fonttype);
        
        Vector3 pen_pos = Vector3::Zero; //This is where alignment comes to play
        // Iterate through all characters in the string
        for (char c : text.m_words)
        {
            // Get the glyph for the character
            const Asset::Glyph& glyph = asset_font.GetGlyph(c);

            // Render the glyph at the current position
            RenderGlyph(glyph, pen_pos, text.m_scale, text.m_color);

            // Advance to the next character's position
            pen_pos.x -= glyph.advance + 10.0f;/* * 10;*/ // Advance is typically in 1/64 pixels
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLTextRenderer::RenderGlyph(const Asset::Glyph& glyph, Vector2 pos, float scale, const Vector3& color)
    {
        // Bind the texture for the glyph
        glBindTexture(GL_TEXTURE_2D, glyph.textureID);

        // Calculate position and size of the glyph
        float xpos = pos.x - glyph.bearing.x; //Might need fix
        float ypos = pos.y - (glyph.bearing.y - glyph.size.y); // Align glyph to baseline
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
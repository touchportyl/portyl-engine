/*!************************************************************************
* WLVERSE [https://wlverse.web.app]
* openglspriterenderer.cpp
*
* This file implements the OpenGLSpriteRenderer class, which is responsible
* for handling 2D sprite rendering within the game engine. It provides
* functions for rendering sprites, applying post-processing effects,
* and managing the necessary OpenGL resources such as shaders and
* framebuffers.
*
* Key functionalities include:
* - Rendering 2D sprites with texture binding and transformations.
* - Supporting post-processing effects such as Gaussian Blur and Bloom.
* - Providing wrapper functions for commonly used OpenGL operations,
*   ensuring streamlined usage across the codebase.
*
* The renderer is built with a focus on performance and flexibility,
* allowing for easy customization and extension of rendering capabilities.
*
* AUTHORS
* [100%] Soh Wei Jie (weijie.soh@digipen.edu)
*   - Main Author
*   - Developed the core rendering functionalities and post-processing
*     pipeline, ensuring compatibility with the game engine's architecture.
*
* Copyright (c) 2024 DigiPen, All rights reserved.
**************************************************************************/
#include "openglspriterenderer.h"

#include "FlexEngine/AssetManager/assetmanager.h" // FLX_ASSET_GET
#include "FlexEngine/DataStructures/freequeue.h"

namespace FlexEngine
{
    // static member initialization
    uint32_t OpenGLSpriteRenderer::m_draw_calls = 0;
    uint32_t OpenGLSpriteRenderer::m_draw_calls_last_frame = 0;
    uint32_t OpenGLSpriteRenderer::m_maxInstances = 3000; //Should be more than enough
    bool OpenGLSpriteRenderer::m_depth_test = false;
    bool OpenGLSpriteRenderer::m_blending = false;

    std::vector<VertexBufferObject> OpenGLSpriteRenderer::m_vbos;
    std::vector<GLuint> OpenGLSpriteRenderer::m_batchSSBOs;

    std::filesystem::path curr_file_path = __FILE__;
    std::filesystem::path shared_vert_path(curr_file_path.parent_path() / "../../../../assets/shader/Shared.vert");
    std::filesystem::path bloom_brightness_frag_path(curr_file_path.parent_path() / "../../../../assets/shader/bloom/bloom_bright_extraction.frag");
    std::filesystem::path bloom_blur_frag_path(curr_file_path.parent_path() / "../../../../assets/shader/bloom/bloom_gaussian_blurN.frag");
    std::filesystem::path bloom_final_frag_path(curr_file_path.parent_path() / "../../../../assets/shader/bloom/bloom_final_composite.frag");
    Asset::Shader OpenGLSpriteRenderer::m_bloom_brightness_shader;
    Asset::Shader OpenGLSpriteRenderer::m_bloom_gaussianblur_shader;
    Asset::Shader OpenGLSpriteRenderer::m_bloom_finalcomp_shader;

    //////////////////////////////////////////////////////////////
    //GLuint OpenGLSpriteRenderer::samples = 8;
    //float OpenGLSpriteRenderer::gamma = 2.2f;
    float OpenGLSpriteRenderer::m_PPopacity = 0.8f;

    GLuint OpenGLSpriteRenderer::m_editorFBO = 0;
    GLuint OpenGLSpriteRenderer::m_postProcessingFBO = 0;
    GLuint OpenGLSpriteRenderer::m_bloomFBO = 0;

    GLuint OpenGLSpriteRenderer::m_pingpongTex[2] = {};
    GLuint OpenGLSpriteRenderer::m_postProcessingTex = 0;
    GLuint OpenGLSpriteRenderer::m_editorTex = {};
    GLuint OpenGLSpriteRenderer::m_finalRenderTex = {};

    float width, height;
    //////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////////////////
    // WRAPPER FUNCTIONS
    /*!***************************************************************************
    * \brief
    * Retrieves the total number of draw calls made.
    *
    * \return Total draw calls as a uint32_t.
    *****************************************************************************/
    uint32_t OpenGLSpriteRenderer::GetDrawCalls() { return m_draw_calls; }
    /*!***************************************************************************
    * \brief
    * Retrieves the number of draw calls made in the last frame.
    *
    * \return Total draw calls from the last frame as a uint32_t.
    *****************************************************************************/
    uint32_t OpenGLSpriteRenderer::GetDrawCallsLastFrame() { return m_draw_calls_last_frame; }

    void OpenGLSpriteRenderer::SetDefaultFrameBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
    void OpenGLSpriteRenderer::SetEditorFrameBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, m_editorFBO); }
    void OpenGLSpriteRenderer::SetPPFrameBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, m_postProcessingFBO); }
    void OpenGLSpriteRenderer::SetBloomFrameBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, m_bloomFBO); }

    /*!***************************************************************************
    * \brief
    * Checks if depth testing is enabled.
    *
    * \return True if depth testing is enabled, otherwise false.
    *****************************************************************************/
    bool OpenGLSpriteRenderer::IsDepthTestEnabled() { return m_depth_test; }
    /*!***************************************************************************
    * \brief
    * Enables depth testing for rendering.
    *****************************************************************************/
    void OpenGLSpriteRenderer::EnableDepthTest()
    {
        m_depth_test = true;
        glEnable(GL_DEPTH_TEST);
    }
    /*!***************************************************************************
    * \brief
    * Disables depth testing for rendering.
    *****************************************************************************/
    void OpenGLSpriteRenderer::DisableDepthTest()
    {
        m_depth_test = false;
        glDisable(GL_DEPTH_TEST);
    }
    /*!***************************************************************************
    * \brief
    * Checks if blending is enabled.
    *
    * \return True if blending is enabled, otherwise false.
    *****************************************************************************/
    bool OpenGLSpriteRenderer::IsBlendingEnabled() { return m_blending; }
    /*!***************************************************************************
    * \brief
    * Enables blending for rendering.
    *****************************************************************************/
    void OpenGLSpriteRenderer::EnableBlending()
    {
        m_blending = true;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    /*!***************************************************************************
    * \brief
    * Disables blending for rendering.
    *****************************************************************************/
    void OpenGLSpriteRenderer::DisableBlending()
    {
        m_blending = false;
        glDisable(GL_BLEND);
    }
    /*!***************************************************************************
    * \brief
    * Clears the current framebuffer.
    *****************************************************************************/
    void OpenGLSpriteRenderer::ClearFrameBuffer() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
    /*!***************************************************************************
    * \brief
    * Clears the color buffer with a specified color.
    *
    * \param color The color to clear the buffer to, represented as a Vector4.
    *****************************************************************************/
    void OpenGLSpriteRenderer::ClearColor(const Vector4& color)
    {
        glClearColor(color.x, color.y, color.z, color.w);
        m_draw_calls_last_frame = m_draw_calls;
        m_draw_calls = 0;
    }
    /*!***************************************************************************
    * \brief
    * Retrieves the VAO ID for a specified VBO type.
    *
    * \param type The type of VBO for which to retrieve the VAO ID.
    * \return The corresponding VAO ID as a GLuint.
    *****************************************************************************/
    GLuint OpenGLSpriteRenderer::GetVAO_ID(Renderer2DProps::VBO_Type type) { return m_vbos[type].vao; }

    GLuint OpenGLSpriteRenderer::GetCreatedTexture(CreatedTextureID id)
    {
        switch (id)
        {
        case CreatedTextureID::CID_finalRender:
            return m_finalRenderTex;
        case CreatedTextureID::CID_blur:
            return m_pingpongTex[0];
        case CreatedTextureID::CID_editor:
        default:
            return m_editorTex;
        }
    }
    //////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////

    /*!***************************************************************************
    * \brief
    * Creates a VAO and VBO with the specified vertices.
    *
    * Pls enter in the following sample vertices order:
    * float vertices[] = {
    * // Position        // TexCoords
    * -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // Bottom-left
    *  0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // Bottom-right
    *  0.5f,  0.5f, 0.0f,   0.0f, 1.0f, // Top-right
    *  0.5f,  0.5f, 0.0f,   0.0f, 1.0f, // Top-right
    * -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // Top-left
    * -0.5f, -0.5f, 0.0f,   1.0f, 0.0f  // Bottom-left
    * };
    *
    * \param vao Reference to the VAO ID to be created.
    * \param vbo Reference to the VBO ID to be created.
    * \param vertices Pointer to an array of vertex data.
    * \param vertexCount The number of vertices in the array.
    *****************************************************************************/
    void OpenGLSpriteRenderer::InitQuadVAO_VBO(GLuint& vao, GLuint& vbo, const float* vertices, int vertexCount)
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

        glBindVertexArray(0);

        // free in freequeue
        FreeQueue::Push(
          [=]()
        {
            glDeleteBuffers(1, &vbo);
            glDeleteVertexArrays(1, &vao);
        }
        );
    }

    void OpenGLSpriteRenderer::InitBatchSSBO()
    {
        // Set up the SSBO for instance data
        GLuint t_tempSSBO;
        glGenBuffers(1, &t_tempSSBO);
        m_batchSSBOs.emplace_back(t_tempSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, t_tempSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_maxInstances * sizeof(Matrix4x4), nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, t_tempSSBO);

        glGenBuffers(1, &t_tempSSBO);
        m_batchSSBOs.emplace_back(t_tempSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, t_tempSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_maxInstances * sizeof(Vector3), nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, t_tempSSBO);

        glGenBuffers(1, &t_tempSSBO);
        m_batchSSBOs.emplace_back(t_tempSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, t_tempSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_maxInstances * sizeof(Vector3), nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, t_tempSSBO);


        FreeQueue::Push(
        [=]()
        {
            for (GLuint iter : m_batchSSBOs)
                glDeleteBuffers(1, &iter);
        }
        );
    }
    /*!***************************************************************************
    * \brief
    * Initializes the VBOs.
    *
    * \param windowSize The size of the rendering window as a Vector2.
    *****************************************************************************/
    void OpenGLSpriteRenderer::Init(const Vector2& windowSize)
    {
        /////////////////////////////////////////////////////////////////////////////////////
        // Create VAOs and VBOs (CAN BE DONE BETTER)
        struct VertexData {
            VertexBufferObject buffer;
            const float* vertices;
            int count;
        };

        float quadVertices[] = {
            // Positions         // TexCoords
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
             0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f
        };
        float quadInvertedVertices[] = {
            // Positions         // TexCoords
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
             0.5f,  0.5f, 0.0f, 0.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 0.0f, 0.0f,
            -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f
        };
        float lineVertices[] =
        {
            // Positions         // TexCoords
            -0.5f, -0.5f, 0.0f, 50.0f, 0.0f,
             0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 0.0f, 48.0f,
             0.5f,  0.5f, 0.0f, 0.0f, 48.0f,
            -0.5f,  0.5f, 0.0f, 48.0f, 48.0f,
            -0.5f, -0.5f, 0.0f, 48.0f, 0.0f
        };
        float postProcessingVertices[] =
        {
            // Positions         // TexCoords
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f
        };

        VertexData vertexData[] =
        {
            {VertexBufferObject(), quadVertices, sizeof(quadVertices) / sizeof(float)},
            {VertexBufferObject(), quadInvertedVertices, sizeof(quadInvertedVertices) / sizeof(float)},
            {VertexBufferObject(), lineVertices, sizeof(lineVertices) / sizeof(float)},
            {VertexBufferObject(), postProcessingVertices, sizeof(postProcessingVertices) / sizeof(float)}
        };

        for (auto& data : vertexData)
        {
            InitQuadVAO_VBO(data.buffer.vao, data.buffer.vbo, data.vertices, data.count);
            m_vbos.push_back(data.buffer);
        }

        Log::Info("All VAOs & VBOs are set up.");

        /////////////////////////////////////////////////////////////////////////////////////
        // Create SSBOs
        InitBatchSSBO();
        Log::Info("All SSBOs are set up.");

        /////////////////////////////////////////////////////////////////////////////////////
        // Linking shaders
        m_bloom_brightness_shader.Create(shared_vert_path, bloom_brightness_frag_path);
        m_bloom_gaussianblur_shader.Create(shared_vert_path, bloom_blur_frag_path);
        m_bloom_finalcomp_shader.Create(shared_vert_path, bloom_final_frag_path);
        FreeQueue::Push(
          [=]()
        {
            m_bloom_brightness_shader.Destroy();
            m_bloom_gaussianblur_shader.Destroy();
            m_bloom_finalcomp_shader.Destroy();
        }
        );
        Log::Info("All post-processing shaders are created.");

        //////////////////////////////////////////////////////////////////////////////////////////////////
        // Create relevant FBO 
        GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glGenFramebuffers(1, &m_postProcessingFBO); //For final composite post-process
        SetPPFrameBuffer();
        width = windowSize.x;
        height = windowSize.y;

        // Create Post-processing FBO
        glGenTextures(1, &m_postProcessingTex);
        glBindTexture(GL_TEXTURE_2D, m_postProcessingTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_postProcessingTex, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Log::Fatal("Post-Processing Framebuffer error: " + glCheckFramebufferStatus(GL_FRAMEBUFFER));

        // Create Specialized Post-processing FBO : BLOOM
        glGenFramebuffers(1, &m_bloomFBO); //For gaussian blurring
        SetBloomFrameBuffer();
        glGenTextures(2, m_pingpongTex);
        glBindTexture(GL_TEXTURE_2D, m_pingpongTex[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pingpongTex[0], 0);
        glBindTexture(GL_TEXTURE_2D, m_pingpongTex[1]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_pingpongTex[1], 0);
        glDrawBuffers(2, drawBuffers);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Log::Fatal("Bloom Framebuffer error: " + glCheckFramebufferStatus(GL_FRAMEBUFFER));

        // Create editor FBO
        glGenFramebuffers(1, &m_editorFBO);
        SetEditorFrameBuffer();
        glGenTextures(1, &m_editorTex);
        glBindTexture(GL_TEXTURE_2D, m_editorTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_editorTex, 0);
        glGenTextures(1, &m_finalRenderTex);
        glBindTexture(GL_TEXTURE_2D, m_finalRenderTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_finalRenderTex, 0);
        glDrawBuffers(2, drawBuffers);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Log::Fatal("Editor Framebuffer error: " + glCheckFramebufferStatus(GL_FRAMEBUFFER));

        // Unbind frame buffer
        SetDefaultFrameBuffer();

        FreeQueue::Push(
        [=]()
        {
            glDeleteFramebuffers(1, &m_postProcessingFBO);
            glDeleteFramebuffers(1, &m_editorFBO);
            glDeleteFramebuffers(1, &m_bloomFBO);

            glDeleteTextures(1, &m_postProcessingTex);
            glDeleteTextures(1, &m_editorTex);
            glDeleteTextures(1, &m_finalRenderTex);
            glDeleteTextures(2, m_pingpongTex);
        }
        );
    }

    /*!***************************************************************************
    * \brief
    * Draws a 2D texture using the specified properties.
    *
    * \param props The properties for rendering the texture, including shader,
    *              texture, color, and transformations.
    *****************************************************************************/
    void OpenGLSpriteRenderer::DrawTexture2D(const Renderer2DProps& props)
    {
        // Guard
        if (props.vbo_id >= m_vbos.size() || props.vbo_id < 0)
            Log::Fatal("Vbo_id is invalid. Pls Check or revert to 0.");
        if (props.shader == "" || props.transform == Matrix4x4::Zero)
            return;

        // Bind all
        glBindVertexArray(m_vbos[props.vbo_id].vao);

        // Apply Shader
        auto& asset_shader = FLX_ASSET_GET(Asset::Shader, props.shader);
        asset_shader.Use();

        // Apply Texture
        if (props.texture != "")
        {
            asset_shader.SetUniform_bool("u_use_texture", true);
            auto& asset_texture = FLX_ASSET_GET(Asset::Texture, props.texture);
            //std::cout << props.texture << "\n";
            asset_texture.Bind(asset_shader, "u_texture", 0);
        }
        else
        {
            asset_shader.SetUniform_bool("u_use_texture", false);
            asset_shader.SetUniform_vec3("u_color", props.color_to_add);
        }

        asset_shader.SetUniform_vec3("u_color_to_add", props.color_to_add);
        asset_shader.SetUniform_vec3("u_color_to_multiply", props.color_to_multiply);

        // Transformation & Orthographic Projection
        asset_shader.SetUniform_mat4("u_model", props.transform);
        static const Matrix4x4 view_matrix = Matrix4x4::LookAt(Vector3::Zero, Vector3::Forward, Vector3::Up);
        Matrix4x4 projection_view = Matrix4x4::Orthographic(
          0.0f, props.window_size.x,
          props.window_size.y, 0.0f,
          -2.0f, 2.0f
        ) * view_matrix;
        asset_shader.SetUniform_mat4("u_projection_view", projection_view);

        // Draw
        glDrawArrays(GL_TRIANGLES, 0, 6);
        m_draw_calls++;

        glBindVertexArray(0);
    }

    void OpenGLSpriteRenderer::DrawTexture2D(GLuint TextureID, const Renderer2DProps& props)
    {
        // Guard
        if (props.vbo_id >= m_vbos.size() || props.vbo_id < 0)
            Log::Fatal("Vbo_id is invalid. Pls Check or revert to 0.");
        if (props.shader == "" || props.transform == Matrix4x4::Zero)
            return;

        // Bind all
        glBindVertexArray(m_vbos[props.vbo_id].vao);

        // Apply Shader
        auto& asset_shader = FLX_ASSET_GET(Asset::Shader, props.shader);
        asset_shader.Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureID); // Original scene texture

        // Apply Texture
        asset_shader.SetUniform_bool("u_use_texture", true);
        asset_shader.SetUniform_int("u_texture", 0);
        asset_shader.SetUniform_vec3("u_color_to_add", props.color_to_add);
        asset_shader.SetUniform_vec3("u_color_to_multiply", props.color_to_multiply);

        // Transformation & Orthographic Projection
        asset_shader.SetUniform_mat4("u_model", props.transform);
        static const Matrix4x4 view_matrix = Matrix4x4::LookAt(Vector3::Zero, Vector3::Forward, Vector3::Up);
        Matrix4x4 projection_view = Matrix4x4::Orthographic(
          0.0f, props.window_size.x,
          props.window_size.y, 0.0f,
          -2.0f, 2.0f
        ) * view_matrix;
        asset_shader.SetUniform_mat4("u_projection_view", projection_view);

        // Draw
        glDrawArrays(GL_TRIANGLES, 0, 6);
        m_draw_calls++;

        glBindVertexArray(0);
    }

    void OpenGLSpriteRenderer::DrawBatchTexture2D(const Renderer2DProps& props, const BatchInstanceBlock& data)
    {
        // Guard
        if (data.m_transformationData.size() != data.m_colorAddData.size() ||
            data.m_colorAddData.size() != data.m_colorMultiplyData.size())
        {
            Log::Fatal("Instance batch data block is invalid (Check if all vectors are of same size)");
        }
        else if (data.m_transformationData.size() < 1)
        {
            Log::Debug("Instance batch data block is empty. Should not run render on this texture");
            return;
        }
        if (props.shader == "")
            return;
        GLsizei dataSize = data.m_transformationData.size();

        // Bind all
        glBindVertexArray(m_vbos[props.vbo_id].vao);

        // Apply Shader
        auto& asset_shader = FLX_ASSET_GET(Asset::Shader, "\\shaders\\batch2.0");
        asset_shader.Use();

        // Apply Texture
        if (props.texture != "")
        {
            asset_shader.SetUniform_bool("u_use_texture", true);
            auto& asset_texture = FLX_ASSET_GET(Asset::Texture, props.texture);
            asset_texture.Bind(asset_shader, "u_texture", 0);
        }
        else
        {
            asset_shader.SetUniform_bool("u_use_texture", false);
        }

        //Apply SSBOs
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_batchSSBOs[0]);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, dataSize * sizeof(Matrix4x4), data.m_transformationData.data());
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_batchSSBOs[1]);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, dataSize * sizeof(Vector3), data.m_colorAddData.data());
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_batchSSBOs[2]);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, dataSize * sizeof(Vector3), data.m_colorMultiplyData.data());
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        // Orthographic Projection
        static const Matrix4x4 view_matrix = Matrix4x4::LookAt(Vector3::Zero, Vector3::Forward, Vector3::Up);
        Matrix4x4 projection_view = Matrix4x4::Orthographic(
          0.0f, props.window_size.x,
          props.window_size.y, 0.0f,
          -2.0f, 2.0f
        ) * view_matrix;
        asset_shader.SetUniform_mat4("u_projection_view", projection_view);

        // Draw
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, dataSize);
        m_draw_calls++;

        glBindVertexArray(0);
    }

    /*!***************************************************************************
    * \brief
    * Draws the post-processing layer after all other rendering operations.
    *****************************************************************************/
    void OpenGLSpriteRenderer::DrawPostProcessingLayer()
    {
        SetPPFrameBuffer(); // Initial Frame Buffer Setup

        //CURRENT POST-PROCESS(PP) BEING HANDLED:
        // 1) Reflection / Gloss
        {
            //Pending
        }
        // 2) Bloom
        {
            // Step 1: Set to bloom frame buffer -> you do not want to mess with the other effects textures
            SetBloomFrameBuffer();

            // Step 2: Brightness Extraction
            ApplyBrightnessPass(0.55f);

            // Step 3: Gaussian Blur
            ApplyGaussianBlur(4, 10.0f, 12);

            // Step 4: Final Composition
            ApplyBloomFinalComposition(m_PPopacity);
        }
        // 3) Blur / Focal Adjust
        {
            //Pending
        }
        // 4) Lens Flare
        {
            //Pending
        }

        // Clean-up
        GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, drawBuffers);
        glBindVertexArray(0);
    }

    // Brightness extraction pass
    void OpenGLSpriteRenderer::ApplyBrightnessPass(float threshold)
    {
        GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, drawBuffers);

        m_bloom_brightness_shader.Use();
        m_bloom_brightness_shader.SetUniform_float("u_Threshold", threshold);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_finalRenderTex); // Input: scene texture
        m_bloom_brightness_shader.SetUniform_int("scene", 0);

        glBindVertexArray(m_vbos[Renderer2DProps::VBO_Type::VBO_PProcessing].vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        m_draw_calls++;
    }

    // Gaussian blur pass with ping-pong technique
    void OpenGLSpriteRenderer::ApplyGaussianBlur(int blurDrawPasses, float blurDistance, int intensity)
    {
        m_bloom_gaussianblur_shader.Use();
        bool horizontal = true;

        for (int i = 0; i < blurDrawPasses; ++i)
        {
            GLenum drawBuffer = horizontal ? GL_COLOR_ATTACHMENT0 : GL_COLOR_ATTACHMENT1;
            glDrawBuffers(1, &drawBuffer);

            m_bloom_gaussianblur_shader.SetUniform_int("horizontal", horizontal);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_pingpongTex[horizontal]);
            m_bloom_gaussianblur_shader.SetUniform_int("scene", 0);
            m_bloom_gaussianblur_shader.SetUniform_float("blurDistance", blurDistance);
            m_bloom_gaussianblur_shader.SetUniform_int("intensity", intensity);

            glBindVertexArray(m_vbos[Renderer2DProps::VBO_Type::VBO_PProcessing].vao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            m_draw_calls++;

            horizontal = !horizontal;
        }
    }

    // Final composition pass
    void OpenGLSpriteRenderer::ApplyBloomFinalComposition(float opacity)
    {
        SetEditorFrameBuffer();
        //GLenum drawBuffer = GL_COLOR_ATTACHMENT1;
        GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(1, drawBuffers);

        m_bloom_finalcomp_shader.Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_editorTex); // Original scene texture
        m_bloom_finalcomp_shader.SetUniform_int("screenTex", 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_pingpongTex[0]); // Blur Vertical
        m_bloom_finalcomp_shader.SetUniform_int("bloomVTex", 1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_pingpongTex[1]); // Blur Horizontal
        m_bloom_finalcomp_shader.SetUniform_int("bloomHTex", 2);
        m_bloom_finalcomp_shader.SetUniform_float("opacity", opacity);

        glBindVertexArray(m_vbos[Renderer2DProps::VBO_Type::VBO_PProcessing].vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        m_draw_calls++;
    }
}
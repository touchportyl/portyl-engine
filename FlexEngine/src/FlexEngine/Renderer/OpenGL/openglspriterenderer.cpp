/*!************************************************************************
// WLVERSE [https://wlverse.web.app]
// openglspriterenderer.cpp
//
// This file implements the OpenGLSpriteRenderer class, which is responsible
// for handling 2D sprite rendering within the game engine. It provides
// functions for rendering sprites, applying post-processing effects,
// and managing the necessary OpenGL resources such as shaders and
// framebuffers.
//
// Key functionalities include:
// - Rendering 2D sprites with texture binding and transformations.
// - Supporting post-processing effects such as Gaussian Blur and Bloom.
// - Providing wrapper functions for commonly used OpenGL operations,
//   ensuring streamlined usage across the codebase.
//
// The renderer is built with a focus on performance and flexibility,
// allowing for easy customization and extension of rendering capabilities.
//
// AUTHORS
// [100%] Soh Wei Jie (weijie.soh@digipen.edu)
//   - Main Author
//   - Developed the core rendering functionalities and post-processing
//     pipeline, ensuring compatibility with the game engine's architecture.
//
// Copyright (c) 2024 DigiPen, All rights reserved.
**************************************************************************/
#include "openglspriterenderer.h"

#include "FlexEngine/AssetManager/assetmanager.h" // FLX_ASSET_GET
#include "FlexEngine/DataStructures/freequeue.h"

namespace FlexEngine
{
    // static member initialization
    uint32_t OpenGLSpriteRenderer::m_draw_calls = 0; 
    uint32_t OpenGLSpriteRenderer::m_draw_calls_last_frame = 0;
    bool OpenGLSpriteRenderer::m_depth_test = false;
    bool OpenGLSpriteRenderer::m_blending = false;

    std::vector<VertexBufferObject> OpenGLSpriteRenderer::m_vbos;

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
    GLuint OpenGLSpriteRenderer::m_postProcessingFBO = 0;
    GLuint OpenGLSpriteRenderer::m_pingpongFBO[2] = {};
    GLuint OpenGLSpriteRenderer::m_editorFBO = 0;

    GLuint OpenGLSpriteRenderer::m_brightnessTex = 0;
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

    void OpenGLSpriteRenderer::SetPPFrameBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, m_postProcessingFBO);}
    void OpenGLSpriteRenderer::SetDefaultFrameBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
    void OpenGLSpriteRenderer::SetEditorFrameBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, m_editorFBO); }
    
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
        case CreatedTextureID::CID_brightnessPass:
            return m_brightnessTex;
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
        VertexBufferObject basic;
        float vert[] = {
            // Position           // TexCoords
            -0.5f, -0.5f, 0.0f,   1.0f, 0.0f,  // Bottom-left
             0.5f, -0.5f, 0.0f,   0.0f, 0.0f,  // Bottom-right
             0.5f,  0.5f, 0.0f,   0.0f, 1.0f,  // Top-right
             0.5f,  0.5f, 0.0f,   0.0f, 1.0f,  // Top-right
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f,  // Top-left
            -0.5f, -0.5f, 0.0f,   1.0f, 0.0f   // Bottom-left
        };
        InitQuadVAO_VBO(basic.vao, basic.vbo, vert, sizeof(vert) / sizeof(float));
        m_vbos.push_back(basic);

        VertexBufferObject basicinverted;
        float vert_1[] = {
            // Position           // TexCoords
            -0.5f, -0.5f, 0.0f,   1.0f, 1.0f,  // Bottom-left
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f,  // Bottom-right
             0.5f,  0.5f, 0.0f,   0.0f, 0.0f,  // Top-right
             0.5f,  0.5f, 0.0f,   0.0f, 0.0f,  // Top-right
            -0.5f,  0.5f, 0.0f,   1.0f, 0.0f,  // Top-left
            -0.5f, -0.5f, 0.0f,   1.0f, 1.0f   // Bottom-left
        };
        InitQuadVAO_VBO(basicinverted.vao, basicinverted.vbo, vert_1, sizeof(vert_1) / sizeof(float));
        m_vbos.push_back(basicinverted);

        VertexBufferObject line;
        float vert_2[] = {
            // Position           // TexCoords
            -0.5f, -0.5f, 0.0f,   50.0f, 0.0f,  // Bottom-left
             0.5f, -0.5f, 0.0f,   0.0f, 0.0f,  // Bottom-right
             0.5f,  0.5f, 0.0f,   0.0f, 48.0f,  // Top-right
             0.5f,  0.5f, 0.0f,   0.0f, 48.0f,  // Top-right
            -0.5f,  0.5f, 0.0f,   48.0f, 48.0f,  // Top-left
            -0.5f, -0.5f, 0.0f,   48.0f, 0.0f   // Bottom-left
        };
        InitQuadVAO_VBO(line.vao, line.vbo, vert_2, sizeof(vert_2) / sizeof(float));
        m_vbos.push_back(line);

        VertexBufferObject PProcessing;
        float vert_3[] = {
            // Position        // TexCoords
            -1.0f, -1.f, 0.0f,   0.0f, 0.0f, // Bottom-left
             1.0f, -1.f, 0.0f,   1.0f, 0.0f, // Bottom-right
             1.0f,  1.f, 0.0f,   1.0f, 1.0f, // Top-right
             1.0f,  1.f, 0.0f,   1.0f, 1.0f, // Top-right
            -1.0f,  1.f, 0.0f,   0.0f, 1.0f, // Top-left
            -1.0f, -1.f, 0.0f,   0.0f, 0.0f  // Bottom-left
        };
        InitQuadVAO_VBO(PProcessing.vao, PProcessing.vbo, vert_3, sizeof(vert_3) / sizeof(float));
        m_vbos.push_back(PProcessing);

        Log::Info("All VAOs & VBOs are setup.");

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
        glGenFramebuffers(1, &m_postProcessingFBO); //For final composite post-process
        SetPPFrameBuffer();
        width = windowSize.x;
        height = windowSize.y;
        // Create brightness pass texture
        glGenTextures(1, &m_brightnessTex);
        glBindTexture(GL_TEXTURE_2D, m_brightnessTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_brightnessTex, 0);
        // Create final composite texture (All post_processed rendered on)
        glGenTextures(1, &m_postProcessingTex);
        glBindTexture(GL_TEXTURE_2D, m_postProcessingTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_postProcessingTex, 0);
        unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, attachments);
        auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
            Log::Fatal("Post-Processing Framebuffer error: " + fboStatus);

        // Create gaussian blur texture (All post_processed rendered on)
        glGenFramebuffers(2, m_pingpongFBO); //For gaussian blurring
        glGenTextures(2, m_pingpongTex);
        for (unsigned int i = 0; i < 2; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_pingpongFBO[i]);
            glBindTexture(GL_TEXTURE_2D, m_pingpongTex[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pingpongTex[i], 0);

            fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
                Log::Fatal("Ping-Pong Framebuffer error: " + fboStatus);
        }
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
        unsigned int attachments2[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, attachments2);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Log::Fatal("Editor Framebuffer error: " + fboStatus);

        // Unbind frame buffer
        SetDefaultFrameBuffer();

        FreeQueue::Push(
        [=]()
        {
            glDeleteFramebuffers(1, &m_postProcessingFBO);
            glDeleteFramebuffers(1, &m_editorFBO);
            glDeleteFramebuffers(2, m_pingpongFBO);
            glDeleteTextures(1, &m_brightnessTex);
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
            asset_texture.Bind(asset_shader, "u_texture", 0);
        }
        else if (props.color != Vector3::Zero)
        {
            asset_shader.SetUniform_bool("u_use_texture", false);
            asset_shader.SetUniform_vec3("u_color", props.color);
        }
        else
        {
            Log::Fatal("No texture or color specified for texture shader.");
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
            // Step 1: Brightness Extraction
            ApplyBrightnessPass(0.55f);

            // Step 2: Gaussian Blur
            ApplyGaussianBlur(4, 10.0f, 12);

            // Step 3: Final Composition
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
        m_bloom_brightness_shader.Use();
        m_bloom_brightness_shader.SetUniform_float("u_Threshold", threshold);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_editorTex); // Input: scene texture
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
        glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_brightnessTex, 0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_pingpongFBO[0]);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pingpongTex[0], 0);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_pingpongFBO[1]);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pingpongTex[1], 0);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        for (int i = 0; i < blurDrawPasses; ++i)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_pingpongFBO[horizontal]);
            m_bloom_gaussianblur_shader.SetUniform_int("horizontal", horizontal);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_pingpongTex[!horizontal]);
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
        GLenum drawBuffer = GL_COLOR_ATTACHMENT1;
        glDrawBuffers(1, &drawBuffer);

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



//void OpenGLSpriteRenderer::DrawPostProcessingLayer()
//{
//    SetPPFrameBuffer();
//
//    // Brightness Pass - Extract bright areas
//    m_bloom_brightness_shader.Use();
//    m_bloom_brightness_shader.SetUniform_float("u_Threshold", 0.55f);
//
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, m_editorTex); // Texture rendered in the previous step (scene texture)
//    m_bloom_brightness_shader.SetUniform_int("scene", 0);
//
//    // Render brightness to first ping-pong buffer
//    glBindVertexArray(m_vbos[Renderer2DProps::VBO_Type::VBO_PProcessing].vao);
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//    m_draw_calls++;
//
//    // Step 4: Gaussian Blur Pass
//    m_bloom_gaussianblur_shader.Use();
//    SetPPFrameBuffer();
//    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_brightnessTex, 0);
//    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_pingpongFBO[0]);
//    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pingpongTex[0], 0);
//    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
//    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_pingpongFBO[1]);
//    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pingpongTex[1], 0);
//    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
//    bool horizontal = true;
//    for (int i = 0; i < 4; ++i)
//    {
//        glBindFramebuffer(GL_FRAMEBUFFER, m_pingpongFBO[horizontal]);
//        m_bloom_gaussianblur_shader.SetUniform_int("horizontal", horizontal);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, m_pingpongTex[!horizontal]);
//        m_bloom_gaussianblur_shader.SetUniform_int("scene", 0);
//        m_bloom_gaussianblur_shader.SetUniform_float("blurDistance", 10.0f);
//        m_bloom_gaussianblur_shader.SetUniform_int("intensity", 12);
//
//        glBindVertexArray(m_vbos[Renderer2DProps::VBO_Type::VBO_PProcessing].vao);
//        glDrawArrays(GL_TRIANGLES, 0, 6);
//        m_draw_calls++;
//        horizontal = !horizontal;
//    }
//
//    // Final Composition
//    SetEditorFrameBuffer();
//    GLenum drawBuffer = GL_COLOR_ATTACHMENT1;
//    glDrawBuffers(1, &drawBuffer);
//    //Enable_DefaultFBO_Layer();
//    m_bloom_finalcomp_shader.Use();
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, m_editorTex); // Original scene texture
//    m_bloom_finalcomp_shader.SetUniform_int("screenTex", 0);
//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_2D, m_pingpongTex[0]); // Blur Vertical
//    m_bloom_finalcomp_shader.SetUniform_int("bloomVTex", 1);
//    glActiveTexture(GL_TEXTURE2);
//    glBindTexture(GL_TEXTURE_2D, m_pingpongTex[1]); // Blur Horizontal
//    m_bloom_finalcomp_shader.SetUniform_int("bloomHTex", 2);
//    m_bloom_finalcomp_shader.SetUniform_float("opacity", m_PPopacity);
//
//    glBindVertexArray(m_vbos[Renderer2DProps::VBO_Type::VBO_PProcessing].vao);
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//    m_draw_calls++;
//
//    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
//    glDrawBuffers(2, drawBuffers);
//
//    // Clean-up
//    glBindVertexArray(0);
//}
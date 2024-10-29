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
    uint32_t OpenGLSpriteRenderer::m_maxInstances = 3000; //Should be more than enough
    bool OpenGLSpriteRenderer::m_depth_test = false;
    bool OpenGLSpriteRenderer::m_blending = false;

    std::vector<VertexBufferObject> OpenGLSpriteRenderer::m_vbos;
    GLuint OpenGLSpriteRenderer::m_instanceVBO = 0;
    GLuint OpenGLSpriteRenderer::m_colorVBO = 0;
    GLuint OpenGLSpriteRenderer::m_colorMultiplyVBO = 0;
    GLuint m_quadVAO, m_quadVBO;
    std::vector<Vector3> OpenGLSpriteRenderer::m_colorData = {};
    std::vector<Vector3> OpenGLSpriteRenderer::m_colorMultiplyData = {};
    std::vector<Matrix4x4> OpenGLSpriteRenderer::m_instanceData = {};


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
        // Batch Rendering setup
        m_instanceData.reserve(m_maxInstances);
        // Generate the instance VBO to hold per-instance data (for instancing)
        float vertices[] = {
            // Positions         // Texture Coords
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
             0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f
        };
        
        // Call InitQuadVAO_VBO to set up the quad's VAO and VBO with instance data
        createVAOWithSSBO(m_quadVAO, m_quadVBO, vertices, sizeof(vertices) / sizeof(float));

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




    //BELOW HERE IS STILL IN DEVELOPMENT (NOT WORKING)
    
    void OpenGLSpriteRenderer::BeginBatch() {
        m_instanceData.clear();
    }

    void OpenGLSpriteRenderer::AddToBatch(const Renderer2DProps& props) {
        //// Ensure VBO and shader validation occurs once per batch.
        //if (props.vbo_id >= m_vbos.size() || props.vbo_id < 0) {
        //    Log::Fatal("Invalid VBO ID. Check value or revert to 0.");
        //    return;
        //}
        if (props.shader.empty() || props.transform == Matrix4x4::Zero) {
            return;
        }

        //Renderer2DProps::InstanceData t_instance;
        
        //t_instance.color_to_add = props.m_instancedata.color_to_add;
        //t_instance.color_to_multiply = props.m_instancedata.color_to_multiply;

        //InstanceData instance;
        //instance.transform = props.transform;
        //instance.color = props.color;
        //instance.color_to_add = props.color_to_add;
        //instance.color_to_multiply = props.color_to_multiply;
        //instance.textureID = props.texture;

        m_instanceData.push_back(props.transform);
        m_colorData.push_back(props.color_to_add);
        m_colorMultiplyData.push_back(props.color_to_multiply);
    }

    void OpenGLSpriteRenderer::EndBatch(/*const Renderer2DProps& props*/) {
        if (m_instanceData.empty()) return;

        SetDefaultFrameBuffer();

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_instanceVBO);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, m_instanceData.size() * sizeof(Matrix4x4), m_instanceData.data());
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_colorVBO);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, m_colorData.size() * sizeof(Vector3), m_colorData.data());
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_colorMultiplyVBO);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, m_colorMultiplyData.size() * sizeof(Vector3), m_colorMultiplyData.data());
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        glBindVertexArray(m_quadVAO);
        // Bind and configure shader
        auto& shader = FLX_ASSET_GET(Asset::Shader, "\\shaders\\batch2.0");
        shader.Use();

        // Set texture (if available) for all instances
        auto& texture = FLX_ASSET_GET(Asset::Texture, "\\images\\chess_queen.png");
        texture.Bind(shader, "u_texture", 0);
        shader.SetUniform_bool("u_use_texture", true);
        //shader.SetUniform_vec3("u_color_to_add", m_instanceData[0].color_to_add);
        //shader.SetUniform_vec3("u_color_to_multiply", Vector3::One);
        static const Matrix4x4 view_matrix = Matrix4x4::LookAt(Vector3::Zero, Vector3::Forward, Vector3::Up);
        Matrix4x4 projection_view = Matrix4x4::Orthographic(
          0.0f, 1280,
          750, 0.0f,
          -2.0f, 2.0f
        ) * view_matrix;
        shader.SetUniform_mat4("u_projection_view", projection_view);

        // Render all instances with one draw call
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, static_cast<GLsizei>(m_instanceData.size()));
        m_draw_calls++;

        glBindVertexArray(0);
    }

    //not used
    void OpenGLSpriteRenderer::updateInstanceTransforms(Matrix4x4* mappedBuffer, const std::vector<Matrix4x4>& instanceTransforms) {
        std::memcpy(mappedBuffer, instanceTransforms.data(), instanceTransforms.size() * sizeof(Matrix4x4));
    }
    void OpenGLSpriteRenderer::createVAOWithSSBO(GLuint& vao, GLuint& vbo, const float* vertices, int vertexCount)
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &m_instanceVBO);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices, GL_STATIC_DRAW);

        // Position and Texture Coords (non-instanced attributes)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

        #if 0
        glGenBuffers(1, &m_instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(Matrix4x4), m_instanceData.data(), GL_DYNAMIC_DRAW);
        // Instance attributes (Transform Matrix - 4 x vec4)
        for (unsigned int i = 0; i < 4; i++) {
            glEnableVertexAttribArray(2 + i);
            glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4x4), (void*)(i * sizeof(Vector4)));
            glVertexAttribDivisor(2 + i, 1); // Update every instance
        }
        #endif

        // Set up the SSBO for instance data
        glGenBuffers(1, &m_instanceVBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_instanceVBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_maxInstances * sizeof(Matrix4x4), m_instanceData.data(), GL_DYNAMIC_DRAW);
        // Bind the SSBO to a binding point (e.g., 0)
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_instanceVBO);

        glGenBuffers(1, &m_colorVBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_colorVBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_maxInstances * sizeof(Vector3), m_colorData.data(), GL_DYNAMIC_DRAW);
        // Bind the SSBO to a binding point (e.g., 0)
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_colorVBO);

        glGenBuffers(1, &m_colorMultiplyVBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_colorMultiplyVBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_maxInstances * sizeof(Vector3), m_colorMultiplyData.data(), GL_DYNAMIC_DRAW);
        // Bind the SSBO to a binding point (e.g., 0)
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_colorMultiplyVBO);


        FreeQueue::Push(
        [=]()
        {
            glDeleteBuffers(1, &m_instanceVBO);
            glDeleteBuffers(1, &m_colorVBO);
            glDeleteBuffers(1, &m_colorMultiplyVBO);
            glDeleteBuffers(1, &vbo);
            glDeleteVertexArrays(1, &vao);
        }
        );

        // Instance attributes (Transform Matrix - 4 x vec4)
        //for (unsigned int i = 0; i < 4; i++) {
        //    glEnableVertexAttribArray(2 + i);
        //    glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(i * sizeof(Vector4)));
        //    glVertexAttribDivisor(2 + i, 1); // Update every instance
        //}
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    
    
}
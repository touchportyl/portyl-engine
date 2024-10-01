#pragma once

#include "flx_api.h"
#include "FlexMath/vector4.h"
#include "opengltexture.h"
#include <glad/glad.h>

namespace FlexEngine
{
    struct __FLX_API Renderer2DProps
    {
        enum __FLX_API Alignment
        {
            Alignment_Center = 0,
            Alignment_TopLeft = 1,
            Alignment_TopRight = 2,
            Alignment_BottomLeft = 3,
            Alignment_BottomRight = 4,
        };
        enum __FLX_API VBO_Type
        {
            VBO_Basic = 0,
            VBO_Line = 1,
            VBO_Wireframe = 2,
        };

        std::string shader = R"(/shaders/texture)";
        std::string texture = R"(/images/flexengine/flexengine-256.png)";
        Vector3 color = Vector3(1.0f, 0.0f, 1.0f);
        Vector3 color_to_add = Vector3(0.0f, 0.0f, 0.0f);
        Vector3 color_to_multiply = Vector3(1.0f, 1.0f, 1.0f);
        Matrix4x4 transform = {};
        Vector2 window_size = Vector2(800.0f, 600.0f);
        Alignment alignment = Alignment_Center;
        GLuint vbo_id = 0;
    };

    struct __FLX_API VertexBufferObject
    {
        GLuint vao = 0;
        GLuint vbo = 0;
    };

    class __FLX_API OpenGLSpriteRenderer
    {
        static uint32_t m_draw_calls;
        static uint32_t m_draw_calls_last_frame;
        static bool m_depth_test;
        static bool m_blending;

        static std::vector<VertexBufferObject> m_vbos;

        // Everything below is still testing (ignore if not proper nametypes)
        // Number of samples per pixel for MSAA anti-aliasing for pixel blend - remove jaggedness
        static GLuint samples;
        // Controls the gamma function
        static float gamma; //might be useful for brightening transitioning
        // FBOs and textures for bloom pass
        static GLuint m_rectVAO;
        static GLuint m_rectVBO;
        static GLuint m_postProcessingFBO;
        static GLuint m_colorBuffer;
        static GLuint m_brightBuffer;
        static GLuint bloomTexture;
        static GLuint postProcessingTexture;
        static GLuint m_pingpongFBO[2];
        static GLuint m_pingpongBuffer[2];

    public:
        static uint32_t GetDrawCalls();
        static uint32_t GetDrawCallsLastFrame();

        static bool IsDepthTestEnabled();
        static void EnableDepthTest();
        static void DisableDepthTest();

        static bool IsBlendingEnabled();
        static void EnableBlending();
        static void DisableBlending();

        static void EnablePostProcessing();
        static void DisablePostProcessing();

        static void ClearFrameBuffer();
        static void ClearColor(const Vector4& color);

        /////////////////////////////////////////////////////////////////////
        // Pls enter in the following sample vertices order:
        // float vertices[] = {
        // // Position        // TexCoords
        // -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // Bottom-left
        //  0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // Bottom-right
        //  0.5f,  0.5f, 0.0f,   0.0f, 1.0f, // Top-right
        //  0.5f,  0.5f, 0.0f,   0.0f, 1.0f, // Top-right
        // -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // Top-left
        // -0.5f, -0.5f, 0.0f,   1.0f, 0.0f  // Bottom-left
        // };
        /////////////////////////////////////////////////////////////////////
        static void CreateVAOandVBO(GLuint& vao, GLuint& vbo, const float* vertices, int vertexCount);
        
        // Initialize VBOs for normal FBOs for bloom
        static void Init(const Vector2& windowSize);

        // Draw the texture
        static void DrawTexture2D(const Renderer2DProps& props = {});

        // Draw Post Processing Layer
        static void DrawPostProcessingLayer();
    };
}
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
        };

        std::string shader = R"(/shaders/texture)";
        std::string texture = R"(/images/flexengine/flexengine-256.png)";
        Vector3 color = Vector3(1.0f, 0.0f, 1.0f);
        Vector3 color_to_add = Vector3(0.0f, 0.0f, 0.0f);
        Vector3 color_to_multiply = Vector3(1.0f, 1.0f, 1.0f);
        Vector2 position = Vector2(0.0f, 0.0f);
        Vector2 scale = Vector2(1.0f, 1.0f);
        Vector3 rotation = Vector3(0.0f, 0.0f, 1.0f);
        Vector2 window_size = Vector2(800.0f, 600.0f);
        Alignment alignment = Alignment_Center;
    };

    class __FLX_API OpenGLSpriteRenderer
    {
        static uint32_t m_draw_calls;
        static uint32_t m_draw_calls_last_frame;
        static bool m_depth_test;
        static bool m_blending;


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

        // Initialize FBOs for bloom
        static void InitBloomFBO(const Vector2& windowSize);

        // Draw the texture
        static void DrawTexture2D(const Renderer2DProps& props = {});

        // Draw Post Processing Layer
        static void DrawPostProcessingLayer();

        // Apply bloom (pass bright areas through shader and apply blur)
        static void ApplyBloomEffect();
    };
}
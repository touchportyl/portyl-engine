#pragma once
#include <glad/glad.h>
#include <vector>
#include "FlexMath/Vector4.h"

namespace FlexEngine 
{
    class OpenGLFrameBuffer 
    {
        static GLuint m_editorFBO;
        static GLuint m_postProcessingFBO;
        static GLuint m_bloomFBO;



    public:
        static GLuint m_pingpongTex[2];
        static GLuint m_postProcessingTex;
        static GLuint m_editorTex;
        static GLuint m_finalRenderTex;

        enum __FLX_API CreatedTextureID
        {
            CID_editor,
            CID_finalRender,
            CID_brightnessPass,
            CID_blur,
        };

        static void Init(const Vector2& windowSize);
        static void SetEditorFrameBuffer();
        static void SetDefaultFrameBuffer();
        static void SetPostProcessingFrameBuffer();
        static void SetBloomFrameBuffer();
        static void ClearFrameBuffer();
        static GLuint GetCreatedTexture(GLuint textureID);
    };
}
#pragma once
#include <glad/glad.h>
#include "FlexMath/Matrix4x4.h"
#include "FlexMath/Vector3.h"
#include "opengltexture.h"

namespace FlexEngine 
{
    class OpenGLPostProcessing 
    {
        static GLuint m_VAOid;

        static Asset::Shader m_bloom_brightness_shader;
        static Asset::Shader m_bloom_gaussianblur_shader;
        static Asset::Shader m_bloom_finalcomp_shader;
    public:
        static void Init(GLuint VAOIDtoSet);

        static void ApplyBrightnessPass(float threshold);
        static void ApplyGaussianBlur(int blurPasses, float blurDistance, int intensity);
        static void ApplyBloomFinalComposition(float opacity);
    };
}
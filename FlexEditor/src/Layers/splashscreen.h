#pragma once

#include <FlexEngine.h>
using namespace FlexEngine;

namespace FlexEditor
{

  class SplashScreenLayer : public FlexEngine::Layer
  {
    Asset::Shader shader_texture;
    Asset::Texture img_splash;
    unsigned int VAO{}, VBO{}, EBO{};

  public:
    SplashScreenLayer() : Layer("Flex Editor") {}
    ~SplashScreenLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void Update() override;
  };

}

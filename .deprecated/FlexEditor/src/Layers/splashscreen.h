#pragma once

#include <FlexEngine.h>
using namespace FlexEngine;

namespace FlexEditor
{

  class SplashScreenLayer : public FlexEngine::Layer
  {
    Asset::Shader shader_texture;
    Asset::Mesh mesh_splash;
    Asset::Texture img_splash;

  public:
    SplashScreenLayer() : Layer("Flex Editor") {}
    ~SplashScreenLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void Update() override;
  };

}

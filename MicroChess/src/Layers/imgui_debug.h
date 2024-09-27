#pragma once

#include <FlexEngine.h>

using namespace FlexEngine;

namespace ChronoShift
{

  class DebugLayer : public FlexEngine::Layer
  {
    const Path default_save_directory = Path::current("saves");
    const std::string default_save_name = "default";
    Path current_save_directory = default_save_directory;
    std::string current_save_name = default_save_name;

  public:
    DebugLayer() : Layer("Debug Layer") {}
    ~DebugLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void Update() override;
  };

}

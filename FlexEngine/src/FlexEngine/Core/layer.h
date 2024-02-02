#pragma once

#include <string>

namespace FlexEngine
{

  class Layer
  {
  public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer() = default;

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate() {}
    virtual void OnImGuiRender() {}
  private:
    std::string m_layer_name;
  };

}
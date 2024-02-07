#include <FlexEngine.h>
#include <FlexEngine/Core/EntryPoint.h>

#include "layer_code.h"

namespace FlexEngine
{
  class FlexCode : public Application
  {
  public:
    FlexCode()
      : Application()
    {
      PushLayer(new CodeLayer());
    }
  };
  
  Application* CreateApplication()
  {
    return new FlexCode();
  }
}
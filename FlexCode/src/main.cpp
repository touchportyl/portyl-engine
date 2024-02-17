#include <FlexEngine.h>
#include <FlexEngine/Core/EntryPoint.h>

#include "layer_code.h"

namespace FlexCode
{
  class FlexApplication : public FlexEngine::Application
  {
  public:
    FlexApplication()
      : Application()
    {
      PushLayer(new CodeLayer());
    }
  };

}

namespace FlexEngine
{
  Application* CreateApplication()
  {
    return new FlexCode::FlexApplication();
  }
}
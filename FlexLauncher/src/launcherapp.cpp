#include <FlexEngine.h>
#include <FlexEngine/Core/EntryPoint.h>

#include "layer_launcher.h"

namespace FlexEngine
{
  class FlexLauncher : public Application
  {
  public:
    FlexLauncher()
      : Application()
    {
      PushLayer(new LauncherLayer());
    }
  };
  
  Application* CreateApplication()
  {
    return new FlexLauncher();
  }
}
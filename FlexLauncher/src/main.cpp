#include <FlexEngine.h>
#include <FlexEngine/Core/EntryPoint.h>

#include "layer_launcher.h"

namespace FlexLauncher
{
  class FlexLauncher : public FlexEngine::Application
  {
  public:
    FlexLauncher()
      : Application({ "FlexLauncher", 1280, 690 })
    {
      PushLayer(new LauncherLayer());
    }
  };

}

namespace FlexEngine
{
  Application* CreateApplication()
  {
    return new FlexLauncher::FlexLauncher();
  }
}
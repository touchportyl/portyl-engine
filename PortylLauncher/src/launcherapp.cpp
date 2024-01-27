#include <Portyl/Core/entrypoint.h>

#include "layer_launcher.h"

namespace Portyl
{
  class PortylLauncher : public Application
  {
  public:
    PortylLauncher()
    {
      PushLayer(new LauncherLayer());
    }
  };
  
  Application* CreateApplication()
  {
    return new Application();
  }
}
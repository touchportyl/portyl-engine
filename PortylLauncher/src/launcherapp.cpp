#include <Portyl.h>
#include <Portyl/Core/EntryPoint.h>

#include "layer_launcher.h"

namespace Portyl
{
  class PortylLauncher : public Application
  {
  public:
    PortylLauncher()
      : Application()
    {
      PushLayer(new LauncherLayer());
    }
  };
  
  Application* CreateApplication()
  {
    return new PortylLauncher();
  }
}
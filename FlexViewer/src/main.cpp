#include <FlexEngine.h>
#include <FlexEngine/Core/EntryPoint.h>

#include "layer_viewer.h"

namespace FlexEngine
{
  class FlexViewer : public Application
  {
  public:
    FlexViewer()
      : Application()
    {
      PushLayer(new ViewerLayer());
    }
  };
  
  Application* CreateApplication()
  {
    return new FlexViewer();
  }
}
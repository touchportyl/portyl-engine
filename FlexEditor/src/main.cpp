#include <FlexEngine.h>
#include <FlexEngine/Core/EntryPoint.h>

#include "layer_editor.h"

namespace FlexEngine
{
  class FlexEditor : public Application
  {
  public:
    FlexEditor()
      : Application()
    {
      PushLayer(new EditorLayer());
    }
  };
  
  Application* CreateApplication()
  {
    return new FlexEditor();
  }
}
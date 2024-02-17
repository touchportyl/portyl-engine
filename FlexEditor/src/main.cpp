#include <FlexEngine.h>
#include <FlexEngine/Core/EntryPoint.h>

#include "layer_editor.h"

namespace FlexEditor
{
  class FlexApplication : public FlexEngine::Application
  {
  public:
    FlexApplication()
      : Application()
    {
      PushLayer(new EditorLayer());
    }
  };

}

namespace FlexEngine
{
  Application* CreateApplication()
  {
    return new FlexEditor::FlexApplication();
  }
}
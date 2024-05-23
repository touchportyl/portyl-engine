#include <FlexEngine.h>
#include <FlexEngine/Core/EntryPoint.h>

#include "States.h"

namespace FlexEditor
{
  class FlexApplication : public FlexEngine::Application
  {
  public:
    FlexApplication()
    {
      FlexEngine::ApplicationStateManager::SetState<SplashState>();
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

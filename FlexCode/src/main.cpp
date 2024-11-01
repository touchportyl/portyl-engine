#include <FlexEngine.h>
#include <FlexEngine/Core/EntryPoint.h>

#include "States/mainstate.h"

namespace FlexCode
{
  class FlexApplication : public FlexEngine::Application
  {
  public:
    FlexApplication()
    {
      FlexEngine::ApplicationStateManager::SetState<MainState>();
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
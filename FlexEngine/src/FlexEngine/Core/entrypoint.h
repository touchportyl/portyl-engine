#pragma once

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "application.h"
#include "../flexlogger.h"
#include "../freequeue.h"

/// <summary>
/// Create an application through the chain of inheritance.
/// <para>Use it by creating a new class that inherits from FlexEngine::Application and override the CreateApplication function.</para>
/// </summary>
/// <returns>The application to run</returns>
extern FlexEngine::Application* FlexEngine::CreateApplication();

#pragma warning(suppress : 4100) // unused parameters
int main(int argc, char** argv)
{
  // Enable run-time memory check for debug builds.
#ifdef _DEBUG
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  // Create the logger
  auto log = new FlexEngine::Log();

  // Ensure that the FreeQueue is run at the end of the program
  std::atexit(FlexEngine::FreeQueue::Free);
  
  // Create the application
  auto app = FlexEngine::CreateApplication();

  // Ensure that the application and logger are deleted at the end of the program
  FlexEngine::FreeQueue::Push([&]() { delete app; delete log; });

  // Run the application
  app->Run();

  // Exit the program
  std::exit(0);
}
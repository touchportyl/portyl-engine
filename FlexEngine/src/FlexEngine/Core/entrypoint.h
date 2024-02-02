#pragma once

#include "application.h"

extern FlexEngine::Application* FlexEngine::CreateApplication();

#pragma warning(suppress : 4100)
int main(int argc, char** argv)
{
  auto app = FlexEngine::CreateApplication();
  app->Run();
  delete app;
}
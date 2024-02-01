#pragma once

#include "application.h"

extern Portyl::Application* Portyl::CreateApplication();

#pragma warning(suppress : 4100)
int main(int argc, char** argv)
{
  auto app = Portyl::CreateApplication();
  app->Run();
  delete app;
}
#pragma once

#include "entrypoint.h"

#pragma warning(suppress : 4100)
int main(int argc, char** argv)
{
  auto app = Portyl::CreateApplication();
  app->Run();
  delete app;
}

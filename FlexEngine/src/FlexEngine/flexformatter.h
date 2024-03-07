#pragma once

//#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "datetime.h" // includes iostream

namespace FlexEngine
{

  using Version = unsigned int;

  class FlexFormatter
  {
  public:
    struct Node
    {
    };

    struct SubNode
    {
    };
    
  private:
    static std::string format;
    static Version version;
  };

}
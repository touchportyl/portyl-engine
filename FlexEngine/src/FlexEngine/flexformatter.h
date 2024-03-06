#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip> // for put_time
#include <ctime> // for localtime_s

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

    std::string GetCurrentDate()
    {
      auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      std::tm tm{}; localtime_s(&tm, &now);
      std::stringstream ss;
      ss << std::put_time(&tm, "%Y %m %d");
      return ss.str();
    }
    
  private:
    static std::string format;
    static Version version;
  };

}
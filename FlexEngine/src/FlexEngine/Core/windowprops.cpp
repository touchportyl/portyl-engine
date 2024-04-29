#include "pch.h"

#include "window.h"

namespace FlexEngine
{

  WindowProps::WindowProps(
    const std::string& title,
    int width, int height,
    std::vector<std::pair<int, int>> window_hints,
    const char* opengl_version_text
  )
    : title(title)
    , width(width), height(height)
    , window_hints(window_hints)
    , opengl_version_text(opengl_version_text)
  {
  }

}
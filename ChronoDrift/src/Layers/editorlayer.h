/* Start Header
*****************************************************************/
/*!
WLVERSE [https://wlverse.web.app]
\file      edtiorlayer.h
\author    [100%] Rocky Sutarius
\par       rocky.sutarius\@digipen.edu
\date      24 November 2024
\brief     Editor layer header, declares a layer for the Editor

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header
*******************************************************************/

#pragma once

#include <FlexEngine.h>

using namespace FlexEngine;

namespace ChronoDrift
{

  class EditorLayer : public FlexEngine::Layer
  {
    const Path default_save_directory = Path::current("saves");
    const std::string default_save_name = "default";
    Path current_save_directory = default_save_directory;
    std::string current_save_name = default_save_name;

    bool show_colliders = false;

  public:
    EditorLayer() : Layer("Debug Layer") {}
    ~EditorLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void Update() override;
  };

}

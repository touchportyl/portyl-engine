#pragma once

#include "Reflection/base.h"
#include "gameobject.h"

namespace FlexEngine
{

  class Scene
  { FLX_REFL_SERIALIZABLE FLX_REFL_SERIALIZE
  public:
    GameObject* AddGameObject(std::string name);
    GameObject* GetGameObject(std::string name) const;

    std::unordered_map<std::string, std::shared_ptr<GameObject>> gameobjects;
  };

}
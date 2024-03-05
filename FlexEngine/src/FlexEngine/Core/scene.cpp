#include "scene.h"

namespace FlexEngine
{

  //void Scene::Serialize(std::ostream& stream) const
  //{
  //  stream << "\n=====\nScene\n=====\n";
  //  for (auto& gameobject : gameobjects)
  //  {
  //    stream << "GameObject: " << gameobject.first << "\nComponents:\n";
  //    //gameobject.second->Serialize(stream);
  //  }
  //}

  GameObject* Scene::AddGameObject(std::string name)
  {
    gameobjects[name] = std::make_shared<GameObject>();
    return dynamic_cast<GameObject*>(gameobjects[name].get());
  }

  GameObject* Scene::GetGameObject(std::string name) const
  {
    auto it = gameobjects.find(name);
    if (it != gameobjects.end()) {
      return it->second.get();
    }
    return nullptr;
  }

}

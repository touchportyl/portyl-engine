#include "gameobject.h"

namespace FlexEngine
{

  void GameObject::Serialize(std::ostream& stream) const
  {
    for (auto& component : components)
    {
      stream << component.first.name() << '\n';
      component.second->Serialize(stream);
    }
  }

}
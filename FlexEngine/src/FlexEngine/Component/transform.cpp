#pragma once

#include "transform.h"

namespace FlexEngine
{

  void Transform::Serialize(std::ostream& stream) const
  {
    stream << "x: " << x << std::endl;
  }

}
#pragma once

#include "flx_api.h"

#include "Reflection/base.h"

#include "FlexMath/vector3.h"

namespace FlexEngine
{
  
  class __FLX_API Vertex
  { FLX_REFL_SERIALIZABLE
  public:
    Vector3 position;
    //Vector3 normal;
    Vector3 color;
    Vector2 tex_coords;
    //unsigned int object_id;

    static void SetLayout();
  };

}
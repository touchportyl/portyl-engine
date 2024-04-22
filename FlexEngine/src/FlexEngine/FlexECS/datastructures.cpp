#include "datastructures.h"

namespace FlexEngine
{
  namespace FlexECS
  {

    FLX_REFL_REGISTER_START(Archetype)
      FLX_REFL_REGISTER_PROPERTY(id)
      FLX_REFL_REGISTER_PROPERTY(type)
      FLX_REFL_REGISTER_PROPERTY(archetype_table)
      FLX_REFL_REGISTER_PROPERTY(entities)
      //FLX_REFL_REGISTER_PROPERTY(edges)
    FLX_REFL_REGISTER_END;

    FLX_REFL_REGISTER_START(EntityRecord)
      //FLX_REFL_REGISTER_PROPERTY(archetype)
      FLX_REFL_REGISTER_PROPERTY(archetype_id)
      FLX_REFL_REGISTER_PROPERTY(row)
    FLX_REFL_REGISTER_END;

    FLX_REFL_REGISTER_START(ArchetypeRecord)
      FLX_REFL_REGISTER_PROPERTY(column)
    FLX_REFL_REGISTER_END;

    FLX_REFL_REGISTER_START(Scene)
      FLX_REFL_REGISTER_PROPERTY(archetype_index)
      FLX_REFL_REGISTER_PROPERTY(entity_index)
      FLX_REFL_REGISTER_PROPERTY(component_index)
    FLX_REFL_REGISTER_END;

  }
}
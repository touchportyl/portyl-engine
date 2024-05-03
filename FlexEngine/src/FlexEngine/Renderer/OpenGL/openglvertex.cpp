#include "openglvertex.h"

#include <glad/glad.h>

namespace FlexEngine
{

  #pragma region Reflection

  FLX_REFL_REGISTER_START(Vertex)
    FLX_REFL_REGISTER_PROPERTY(position)
    FLX_REFL_REGISTER_PROPERTY(color)
    FLX_REFL_REGISTER_PROPERTY(tex_coords)
  FLX_REFL_REGISTER_END;

  #pragma endregion

  void Vertex::SetLayout()
  {
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, tex_coords));
  }

}
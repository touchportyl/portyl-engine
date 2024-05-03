#pragma once

#include "flx_api.h"

#include "Renderer/buffer.h"
#include "Renderer/OpenGL/openglvertex.h"
#include "Renderer/OpenGL/opengltextures.h"
#include "Renderer/OpenGL/openglshaders.h"

#include <vector>

namespace FlexEngine
{
  namespace Asset
  {

    // The current implementation of the Mesh class is not optimal.
    // It's only designed to work for single textures.
    class __FLX_API Mesh
    {
    public:
      std::vector<Vertex> vertices;
      std::vector<unsigned int> indices;
      std::vector<Texture> textures;

      std::unique_ptr<VertexArray> VAO;
      std::unique_ptr<VertexBuffer> VBO;
      std::unique_ptr<IndexBuffer> IBO;

      Mesh() = default;
      Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);

      void Draw(const Asset::Shader& shader);
    };

  }
}

#include "openglmesh.h"

namespace FlexEngine
{
  namespace Asset
  {

    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Material& material, const Matrix4x4& transform)
      : vertices(vertices), indices(indices), material(material), transform(transform)
    {
    }

    #pragma region Operator Overloads

    bool Mesh::operator==(const Mesh& other) const
    {
      return vertices == other.vertices && indices == other.indices && material == other.material && transform == other.transform;
    }

    bool Mesh::operator!=(const Mesh& other) const
    {
      return !(*this == other);
    }

    #pragma endregion

    //Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
    //  : vertices(vertices), indices(indices), textures(textures)
    //{
    //  VAO.reset(VertexArray::Create());
    //  VBO.reset(VertexBuffer::Create(this->vertices.data(), vertices.size() * sizeof(Vertex)));
    //  IBO.reset(IndexBuffer::Create(this->indices.data(), static_cast<GLsizei>(indices.size())));
    //}
    //
    //void Mesh::Draw(const Asset::Shader& shader)
    //{
    //  // bind everything
    //  VAO->Bind();
    //  VBO->Bind();
    //  IBO->Bind();
    //
    //  // bind the shader
    //  shader.Use();
    //
    //  // bind the textures
    //  for (unsigned int i = 0; i < textures.size(); i++)
    //  {
    //    textures[i].Bind(i);
    //  }
    //  glActiveTexture(GL_TEXTURE0);
    //
    //  glDrawElements(GL_TRIANGLES, IBO->GetCount(), GL_UNSIGNED_INT, nullptr);
    //
    //  // unbind in reverse order
    //  IBO->Unbind();
    //  VBO->Unbind();
    //  VAO->Unbind();
    //}

  }
}
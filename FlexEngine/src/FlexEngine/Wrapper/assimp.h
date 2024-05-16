#pragma once

#include "flx_api.h"

#include "Wrapper/path.h"
#include "Renderer/OpenGL/openglmaterial.h"
#include "Renderer/OpenGL/openglmodel.h"
#include "FlexMath/matrix4x4.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace FlexEngine
{

  // This is a wrapper for the Assimp library.
  // It provides a simple interface to load 3D models.
  // This implementation is not thread-safe.

  class __FLX_API AssimpWrapper
  {
    static Assimp::Importer importer;
    static unsigned int import_flags;

  public:
    // static class
    AssimpWrapper() = delete;
    AssimpWrapper(const AssimpWrapper&) = delete;
    AssimpWrapper(AssimpWrapper&&) = delete;
    AssimpWrapper& operator=(const AssimpWrapper&) = delete;
    AssimpWrapper& operator=(AssimpWrapper&&) = delete;

    static Asset::Model LoadModel(const Path& path);

  private:
    static std::vector<Asset::Mesh> Internal_ProcessNode(
      aiNode* node,
      const aiScene* scene,
      Matrix4x4 parent_transform,
      std::vector<Asset::Material>* out_materials
    );

    static Asset::Mesh Internal_ProcessMesh(
      aiMesh* mesh,
      const aiScene* scene,
      Matrix4x4 mesh_transform,
      std::vector<Asset::Material>* out_materials,
      const std::string& node_name
    );

    static Asset::Material::TextureVariant Internal_ProcessMaterialTextures(
      aiMaterial* material,
      aiTextureType type,
      const aiScene* scene
    );
  };

}

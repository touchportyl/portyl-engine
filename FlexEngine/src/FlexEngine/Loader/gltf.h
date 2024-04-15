#pragma once

//#include <iostream>
//#include <fstream>
//#include <string>

#include <glm/gtx/quaternion.hpp>

#include <fastgltf/glm_element_traits.hpp>
#include <fastgltf/parser.hpp>
#include <fastgltf/tools.hpp>
#include <fastgltf/types.hpp>

namespace FlexEngine
{
  namespace Asset
  {

    class glTF
    {
      fastgltf::Asset m_asset;

    public:

      // Load a glTF file from a path
      // Adapted from https://fastgltf.readthedocs.io/v0.6.x/overview.html
      // and vulkan-guide
      void Load(const std::filesystem::path& path)
      {
        // Creates a Parser instance. Optimally, you should reuse this across loads, but don't use it
        // across threads. To enable extensions, you have to pass them into the parser's constructor.
        fastgltf::Parser parser;

        // The GltfDataBuffer class is designed for re-usability of the same JSON string. It contains
        // utility functions to load data from a std::filesystem::path, copy from an existing buffer,
        // or re-use an already existing allocation. Note that it has to outlive the process of every
        // parsing function you call.
        fastgltf::GltfDataBuffer data;
        data.loadFromFile(path);

        // gltfOptions is a bitmask of options that can be passed to the parser.
        constexpr auto gltfOptions =
          fastgltf::Options::DontRequireValidAssetMember |
          fastgltf::Options::AllowDouble |
          fastgltf::Options::LoadGLBBuffers |
          fastgltf::Options::LoadExternalBuffers |
          fastgltf::Options::LoadExternalImages |
          fastgltf::Options::GenerateMeshIndices;

        // This loads the glTF file into the gltf object and parses the JSON.
        // For GLB files, use Parser::loadBinaryGLTF instead.
        // You can detect the type of glTF using fastgltf::determineGltfFileType.
        auto type = fastgltf::determineGltfFileType(&data);
        fastgltf::Expected<fastgltf::Asset> load(fastgltf::Error::None);

        switch (type)
        {
        case fastgltf::GltfType::glTF:
          load = parser.loadGLTF(&data, path.parent_path(), gltfOptions);
          break;
        case fastgltf::GltfType::GLB:
          load = parser.loadBinaryGLTF(&data, path.parent_path(), gltfOptions | fastgltf::Options::LoadGLBBuffers);
          break;
        case fastgltf::GltfType::Invalid:
          std::cerr << "Invalid glTF container" << std::endl;
          abort();
        default:
          std::cerr << "Failed to determine glTF container" << std::endl;
          abort();
        }

        if (load)
        {
          m_asset = std::move(load.get());
        }
        else
        { // Error handling
          std::cout << "Failed to load glTF: " << fastgltf::getErrorMessage(load.error()) << std::endl;
          abort();
        }

        // Optionally, you can now also call the fastgltf::validate method. This will more strictly
        // enforce the glTF spec and is not needed most of the time, though I would certainly
        // recommend it in a development environment or when debugging to avoid mishaps.

        //fastgltf::validate(asset.get());
      }
    
    };

  }
}
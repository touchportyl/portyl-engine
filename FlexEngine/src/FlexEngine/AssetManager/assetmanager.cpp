#include "pch.h"

#include "assetmanager.h"

namespace FlexEngine
{

  // static member initialization
  Path AssetManager::default_directory = Path::current_path("assets");
  std::unordered_map<AssetKey, AssetVariant> AssetManager::assets;


  void AssetManager::Load()
  {
    FLX_FLOW_BEGINSCOPE();

    // load all assets
    FileList list = FileList::GetAllFilesInDirectoryRecursively(default_directory);

    // guard
    if (list.size() == 0)
    {
      Log::Info(std::string("No assets found in directory: ") + default_directory.string());
      return;
    }

    // used to create an asset key
    // substr the path starting after the length of the default directory
    std::size_t default_directory_length = default_directory.string().length();

    // used to link shaders together
    // shaders with the same name will be linked together
    // looks for .vert, .frag, and .geom files
    // .glsl and .hlsl files are currently not supported
    // but in the future they will skip this linker because
    // they contain all the necessary information in one file
    std::unordered_map<std::string, std::array<File*, 3>> shader_linker;

    Log::Flow("Loading assets...");
    // iterate through all files in the directory
    list.each(
      [&default_directory_length, &shader_linker](File& file)
      {
        // determine what type of asset it is
        // 
        // currently supported:
        // - textures
        // - shaders

        auto file_extension = file.path.get().extension();

        if (FLX_EXTENSIONS_CHECK_SAFETY("image", file_extension.string()))
        {
          // create an asset key
          AssetKey key = file.path.string().substr(default_directory_length);

          // load texture
          assets[key] = Asset::Texture();
          Asset::Texture& texture = std::get<Asset::Texture>(assets[key]);
          texture.Load(file.path);
          Log::Info(std::string("Loaded texture: ") + key);
        }
        else if (FLX_EXTENSIONS_CHECK_SAFETY("shader", file_extension.string()))
        {
          // ignore .glsl and .hlsl files
          // they are not currently supported
          if (file_extension.string() == ".glsl" || file_extension.string() == ".hlsl") return;

          std::string file_name = file.path.stem().string();

          Asset::Shader::Type shader_type;
          if (file_extension.string() == ".vert")
          {
            shader_type = Asset::Shader::Type::Vertex;
          }
          else if (file_extension.string() == ".frag")
          {
            shader_type = Asset::Shader::Type::Fragment;
          }
          else if (file_extension.string() == ".geom")
          {
            shader_type = Asset::Shader::Type::Geometry;
          }
          else
          {
            Log::Warning(std::string("Unsupported shader type: ") + file_extension.string());
            return;
          }

          // add it to the linker
          // the shader linker will link shaders with the same name after all shaders are loaded
          if (shader_linker.count(file_name) == 0)
          {
            // create a new entry in the linker
            shader_linker[file_name] = {nullptr, nullptr, nullptr};
          }

          // add the file to the linker
          shader_linker[file_name][shader_type] = &file;
        }
      }
    );

    // perform shader linking
    // currently geometry shaders are not supported
    Log::Flow("Linking shaders...");
    for (auto& [key, files] : shader_linker)
    {
      // guard: check if all shaders are present
      if (files[Asset::Shader::Type::Vertex] == nullptr || files[Asset::Shader::Type::Fragment] == nullptr)
      {
        Log::Error(std::string("Shader ") + key + " is missing a vertex or fragment shader!");
        continue;
      }

      // create an asset key
      AssetKey assetkey = files[0]->path.string().substr(default_directory_length);
      assetkey = assetkey.substr(0, assetkey.find_last_of('.'));

      // create a new shader
      assets[assetkey] = Asset::Shader();
      Asset::Shader& shader = std::get<Asset::Shader>(assets[assetkey]);
      shader.Create(files[Asset::Shader::Type::Vertex]->path, files[Asset::Shader::Type::Fragment]->path);
      Log::Info(std::string("Loaded shader: ") + assetkey);
    }

    FLX_FLOW_ENDSCOPE();
  }


  AssetVariant* AssetManager::Get(const AssetKey& key)
  {
    if (assets.count(key) == 0)
    {
      Log::Error(std::string("Asset not found: ") + key);
      return nullptr;
    }
    return &assets[key];
  }

#ifdef _DEBUG
  void AssetManager::Dump()
  {
    Log::Debug("AssetManager:");
    for (auto& [key, asset] : assets)
    {
      Log::Debug("Key: " + key);
      std::visit(
        [](auto&& arg)
        {
          using T = std::decay_t<decltype(arg)>;
          if constexpr (std::is_same_v<T, Asset::Texture>)
          {
            Log::Debug("Type: Texture");
            Log::Debug("Size: [" + std::to_string(arg.GetWidth()) + ", " + std::to_string(arg.GetHeight()) + "]");
          }
          else if constexpr (std::is_same_v<T, Asset::Shader>)
          {
            Log::Debug("Type: Shader");
            arg.Dump();
          }
        },
        asset
      );
      Log::Debug(std::string(10, '-'));
    }
    Log::Debug("End of dump.");
  }
#endif

}

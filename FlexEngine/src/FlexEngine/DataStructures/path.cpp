#include "path.h"

namespace FlexEngine
{

  namespace Extensions
  {
    // Match these file extensions with the FlxFmtFileType enum (flexformatter.h)
    const std::unordered_map<std::string, std::set<std::string>> safe =
    {
      // FlexEngine files
      { "flx", { ".flxscene", ".flxscript", ".flxprefab", ".flxmaterial" } },
      // FlexEngine binary files
      { "flb", { ".flbscene", ".flbscript", ".flbprefab", ".flbmaterial" } },
      // Data files (txt, json, etc.)
      { "data", { ".txt", ".json", ".csv", ".yaml", ".ini", ".cfg", ".log", ".dat" } },
      // Image files (jpg, png, etc.)
      { "image", { ".jpg", ".jpeg", ".png" } },
      // Video files (mp4, avi, etc.)
      { "video", { ".mp4" } },
      // Audio files (mp3, wav, etc.)
      { "audio", { ".mp3", ".wav", ".ogg", ".flac" } },
      // Model files (obj, fbx, etc.)
      { "model", { ".obj", ".mtl", ".fbx", ".gltf", ".glb" } },
    };
  }

  #pragma region Constructors

  // Constructor to ensure validity of the path
  // Throws an exception if the path is invalid
  Path::Path(const std::filesystem::path& _path)
  {
    // Check if the new path is valid, if not, throw an exception
    if (!std::filesystem::exists(_path.root_path()))
    {
      throw std::invalid_argument("Invalid file path");
    }

    // Check if the file is safe/supported
    if (_path.has_extension())
    {
      // Get the extension
      std::string extension = _path.extension().string();

      // Check if the extension is safe
      bool is_safe = false;
      for (const auto& [category, extensions] : Extensions::safe)
      {
        if (extensions.count(extension) != 0)
        {
          is_safe = true;
          break;
        }
      }

      // Throw an exception if the extension is not safe
      if (!is_safe) throw std::invalid_argument("Unsupported file extension: " + extension);
    }

    path = _path;
  }

  #pragma endregion

  #pragma region Getter/setter functions

  std::filesystem::path Path::get() const noexcept
  {
    return path;
  }

  //void Path::set(const std::filesystem::path& _path)
  //{
  //  // Check if the file is safe/supported
  //  if (_path.has_extension())
  //  {
  //    // Get the extension
  //    std::string extension = _path.extension().string();
  //
  //    // Check if the extension is safe
  //    bool is_safe = false;
  //    for (const auto& [category, extensions] : Extensions::safe)
  //    {
  //      if (extensions.count(extension) != 0)
  //      {
  //        is_safe = true;
  //        break;
  //      }
  //    }
  //
  //    // Throw an exception if the extension is not safe
  //    if (!is_safe) throw std::invalid_argument("Unsupported file extension: " + extension);
  //  }
  //
  //  // Check if the new path is valid, if not, throw an exception
  //  if (!exists(_path))
  //  {
  //    throw std::invalid_argument("Invalid file path");
  //  }
  //
  //  this->path = _path;
  //}

  #pragma endregion

  #pragma region Queries

  bool Path::is_file() const noexcept
  {
    return !std::filesystem::is_directory(path);
  }

  bool Path::is_directory() const noexcept
  {
    return std::filesystem::is_directory(path);
  }

  #pragma endregion

  #pragma region Passthrough functions

  bool Path::exists(const Path& _path) noexcept
  {
    return std::filesystem::exists(_path);
  }

  Path Path::current_path()
  {
    return std::filesystem::current_path();
  }

  #pragma endregion

  #pragma region Helper functions

  Path Path::current_path(const std::string& path_to_append)
  {
    return std::filesystem::current_path() / path_to_append;
  }

  #pragma endregion

  #pragma region operator overloads

  Path::operator std::filesystem::path() const noexcept { return path; }

  bool operator==(const Path& lhs, const Path& rhs) noexcept { return lhs.get() == rhs.get(); }
  bool operator!=(const Path& lhs, const Path& rhs) noexcept { return lhs.get() != rhs.get(); }
  bool operator<(const Path& lhs, const Path& rhs) noexcept { return lhs.get() < rhs.get(); }
  bool operator>(const Path& lhs, const Path& rhs) noexcept { return lhs.get() > rhs.get(); }

  std::ostream& operator<<(std::ostream& os, const Path& p) noexcept { return os << p.get(); }
  std::istream& operator>>(std::istream& is, Path& p) noexcept
  {
    std::filesystem::path pth;
    is >> pth;
    p = pth;
    return is;
  }

  #pragma endregion

}

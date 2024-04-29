#pragma once

#include "flx_api.h"

#include <filesystem>
#include <iostream> // std::ostream, std::istream
#include <string>
#include <exception> // std::invalid_argument
#include <unordered_map>
#include <set>

namespace FlexEngine
{

  namespace Extensions
  {
    // All the extensions that are considered safe
    // Extensions have a "." in front, e.g. ".txt"
    // Categories: "flx", "flb", "data", "shader", "image", "video", "audio", "model"
    // Usage: safe["category"]
    __FLX_API extern const std::unordered_map<std::string, std::set<std::string>> safe;
  }

  // Wrapper for std::filesystem::path
  // Makes sure that the path is always valid
  // This is written like a std library class
  // Throws an exception if the path is invalid
  class __FLX_API Path
  {
    std::filesystem::path path;

  public:

    // Constructors

    Path() = default;
    Path(const std::filesystem::path& _path);

    // Getter/setter functions

    std::filesystem::path get() const noexcept;
    //void set(const std::filesystem::path& _path);

    // Queries

    bool is_file() const noexcept;
    bool is_directory() const noexcept;

    // Passthrough functions

    static bool exists(const Path& _path) noexcept;
    static Path current_path();

    // Helper functions

    static Path current_path(const std::string& path_to_append);

    // operator overloads

    operator std::filesystem::path() const noexcept;
    operator std::string() const noexcept;
  };

  // operator overloads

  bool operator==(const Path& lhs, const Path& rhs) noexcept;
  bool operator!=(const Path& lhs, const Path& rhs) noexcept;
  bool operator<(const Path& lhs, const Path& rhs) noexcept;
  bool operator>(const Path& lhs, const Path& rhs) noexcept;

  std::ostream& operator<<(std::ostream& os, const Path& p) noexcept;
  std::istream& operator>>(std::istream& is, Path& p) noexcept;
  

}

namespace std
{
  
  // Specialize std::hash for FlexEngine::FilePath
  template <>
  struct hash<FlexEngine::Path>
  {
    std::size_t operator()(const FlexEngine::Path& p) const
    {
      return std::hash<std::filesystem::path>()(p.get());
    }
  };

  // Specialize std::equal_to for FlexEngine::FilePath
  template <>
  struct equal_to<FlexEngine::Path>
  {
    bool operator()(
      const FlexEngine::Path& lhs,
      const FlexEngine::Path& rhs
    ) const
    {
      return lhs == rhs;
    }
  };

  // Overload std::to_string for FlexEngine::FilePath
  std::string to_string(const FlexEngine::Path& p);

}

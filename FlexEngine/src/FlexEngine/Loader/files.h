#pragma once

#include <iostream>
#include <string>
#include <filesystem>

namespace FlexEngine
{

  // Structure to represent a file
  // Files are defined by their name and path
  // There can never be two files with the same path
  struct File
  {
    std::string name;
    std::filesystem::path path;

    File(const std::string& name, const std::filesystem::path& path)
      : name(name), path(path)
    {
    }

    // prevent copying and moving
    File() = delete;
    File(File&) = delete;
    File(File&&) = delete;
    File(const File&) = delete;
    File& operator=(File&) = delete;
    File& operator=(const File&) = delete;

    std::string ToString() const
    {
      return " [ " + name + " ] " + path.string() + "\n";
    }

    friend std::ostream& operator<<(std::ostream& os, const File& file)
    {
      return os << file.path.filename().string() << "\n";
    }

    // comparison operator for sorting
    friend bool operator<(const File& lhs, const File& rhs) { return lhs.path < rhs.path; }
  };

  // todo: implement a file loader
  // read from a directory and load all files

}

#pragma once

#include <FlexEngine.h>

namespace FlexCode
{

  class CodeLayer : public FlexEngine::Layer
  {
  public:
    CodeLayer();
    ~CodeLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void Update() override;
  };

  class EditableFile
  {
  public:
    std::filesystem::path path{}; // file path object
    std::string name{};           // file name without the extension
    std::string extension{};      // file extension with the dot
    std::string data{};           // data read from the file
    bool is_open = true;         // flag for closing the file
    bool is_dirty = false;       // flag for checking if the file has been modified
    bool to_dock = true;         // flag for docking the window

    // overload the == and != operators to compare files
    bool operator==(const EditableFile& other)
    {
      return path == other.path;
    }
    bool operator!=(const EditableFile& other)
    {
      return path != other.path;
    }

    EditableFile(const std::filesystem::path& path)
      : path(path), name(path.filename().generic_string()), extension(path.extension().generic_string())
    {
      std::ifstream ifs(path);
      FLX_ASSERT(ifs, "Could not open file " + path.string());

      // automatically read the file
      std::string _data((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
      data = _data;
    }
  };

}

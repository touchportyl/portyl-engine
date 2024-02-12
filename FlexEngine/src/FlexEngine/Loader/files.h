#pragma once

#include <fstream>
#include <string>

namespace FlexEngine
{
  class File
  {
  public:
    File() = default;

    // Constructor to directly open the file
    File(const std::string& filename, int flags)
    {
      open(filename, flags);
    }

    // Destructor closes the file
    ~File()
    {
      close();
    }

    /// <summary>
    /// Open a file
    /// </summary>
    /// <param name="filename"></param>
    /// <param name="flags">std::ios flags</param>
    void open(const std::string& filename, int flags);

    /// <summary>
    /// Explicitly close the file
    /// <para>Passing through fstream functionality</para>
    /// </summary>
    void close();

    /// <summary>
    /// Check if the file is open
    /// <para>Passing through fstream functionality</para>
    /// </summary>
    bool is_open() const { return file.is_open(); }

    // Operator overloads to read and write from the file
    File& operator>>(std::string& data);
    File& operator<<(const char* data);
    File& operator<<(std::string const& data) { return operator<<(data.c_str()); }

  private:
    std::fstream file{};
  };
}

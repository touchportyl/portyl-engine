#include "pch.h"

#include "files.h"

namespace FlexEngine
{
    
  void File::open(const std::string& filename, int flags)
  {
    if (is_open())
    {
      close();
    }
    file.open(filename, flags);
    if (!is_open())
    {
      Log::Error("Could not open file.");
    }
  }
  void File::close()
  {
    if (file.is_open())
    {
      file.close();
    }
  }

  // Operator overloads
  File& File::operator>>(std::string& data)
  {
    if (file.is_open())
    {
      file >> data;
    }
    else
    {
      Log::Warning("File not open for reading.");
    }
    return *this;
  }
  File& File::operator<<(const char* data)
  {
    if (file.is_open())
    {
      file << data;
    }
    else
    {
      Log::Warning("File not open for writing.");
    }
    return *this;
  }

}

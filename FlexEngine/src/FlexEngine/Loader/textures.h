#pragma once

#include <string>

namespace FlexEngine
{
  namespace Asset
  {

    class Texture
    {
    public:
      Texture() = default;
      ~Texture();

      // load and unload functions
      void Load();
      void Load(const char* path);
      void Load(const std::string& path) { Load(path.c_str()); }
      void Unload() const;

      // binding functions for OpenGL
      inline void Bind(unsigned int slot = 0) const;
      inline void Unbind() const;

      // == and != operator overloading to compare two textures
      friend bool operator==(const Texture& lhs, const Texture& rhs) { return lhs.GetTexture() == rhs.GetTexture(); }
      friend bool operator!=(const Texture& lhs, const Texture& rhs) { return lhs.GetTexture() != rhs.GetTexture(); }

      // getter functions
      inline unsigned int GetTexture()      const { return m_texture; }
      inline int          GetWidth()        const { return m_width; }
      inline int          GetHeight()       const { return m_height; }
      inline float        GetWidthF()       const { return static_cast<float>(m_width); }
      inline float        GetHeightF()      const { return static_cast<float>(m_height); }
      // helper function for ImGui
      inline void*        GetTextureImGui() const { return (void*)(intptr_t)m_texture; }

    private:
      Texture(const std::string& path);

      std::string m_path{};
      unsigned int m_texture = 0;
      int m_width = 0;
      int m_height = 0;
    };

  }
}

#pragma once

#include "flx_api.h"

#include <string>

// Helper macro to display a texture for ImGui using the ImGui::Image function
// Example usage: ImGui::Image(IMGUI_IMAGE(texture_variable));
#define IMGUI_IMAGE(TEXTURE) TEXTURE.GetTextureImGui(), ImVec2(TEXTURE.GetWidthF(), TEXTURE.GetHeightF())

namespace FlexEngine
{
  namespace Asset
  {

    // Texture class to load and unload textures
    // Wraps around OpenGL texture functions and uses stb_image.h to load images
    // Can be used to load textures from files or create textures from memory
    // Can be used to bind and unbind textures for OpenGL
    class __FLX_API Texture
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

      void Bind(unsigned int slot = 0) const;
      void Unbind() const;

      // == and != operator overloading to compare two textures

      friend bool operator==(const Texture& lhs, const Texture& rhs) { return lhs.GetTexture() == rhs.GetTexture(); }
      friend bool operator!=(const Texture& lhs, const Texture& rhs) { return lhs.GetTexture() != rhs.GetTexture(); }

      // getter functions

      unsigned int GetTexture()      const { return m_texture; }
      int          GetWidth()        const { return m_width; }
      int          GetHeight()       const { return m_height; }
      float        GetWidthF()       const { return static_cast<float>(m_width); }
      float        GetHeightF()      const { return static_cast<float>(m_height); }
      // helper function for ImGui
      void*        GetTextureImGui() const { return (void*)(intptr_t)m_texture; }

    private:
      Texture(const std::string& path);

      std::string m_path{};
      unsigned int m_texture = 0;
      int m_width = 0;
      int m_height = 0;
    };

  }
}

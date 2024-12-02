#pragma once

#include "flx_api.h"
#include "FlexMath/vector4.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>

#include <vector>
#include <unordered_map>

namespace FlexEngine
{
	struct __FLX_API Character {
		GLuint     m_textureID;  // ID handle of the glyph texture
		Vector2	   m_size;       // Size of glyph
		Vector2    m_bearing;    // Offset from baseline to left/top of glyph
		GLuint     m_advance;    // Offset to advance to next glyph

	};

	class __FLX_API OpenGLFreeType
	{

	public:

	};
}
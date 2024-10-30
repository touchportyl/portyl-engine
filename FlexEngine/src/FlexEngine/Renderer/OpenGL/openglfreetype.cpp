#include "openglfreetype.h"


namespace FlexEngine 
{
    #if 0
    OpenGLFreeType::OpenGLFreeType(const std::string& file_name, int font_height, float density, int dpi, float line_spacing) noexcept
    {
        init(file_name, ALPHABET, ARRAY_SIZE(ALPHABET), font_height, density, dpi, line_spacing);
    }

    void OpenGLFreeType::init(const std::string& file_name, const unsigned int* alphabet, unsigned int chars_count, int font_height, float density, int dpi, float line_spacing) noexcept {
        // Setup FreeType and Load Font
        if (!setupFreeType(file_name, font_height, dpi)) return;

        // Texture and Size Allocation
        this->density = density;
        this->height = static_cast<unsigned int>(line_spacing * float(font_height));
        textures.resize(chars_count);
        chars_widths.resize(chars_count);

        // Initialize VAO and VBO for character quads
        initCharacterVAO();

        // Load each character glyph
        for (unsigned int i = 0; i < chars_count; ++i) {
            chars_widths[alphabet[i]] = loadCharacter(alphabet[i]);
        }

        FT_Done_Face(face);
        FT_Done_FreeType(library);
    }

    bool OpenGLFreeType::setupFreeType(const std::string& file_name, int font_height, int dpi) {
        // Initialize FreeType
        if (FT_Init_FreeType(&library) != 0) {
            std::cerr << "Error: FreeType library couldn't be initialized.\n";
            return false;
        }
        if (FT_New_Face(library, file_name.c_str(), 0, &face) != 0) {
            std::cerr << "Error: font file \"" << file_name << "\" couldn't be loaded\n";
            return false;
        }
        FT_Select_Charmap(face, FT_ENCODING_UNICODE);
        FT_Set_Char_Size(face, font_height << 6, font_height << 6, dpi, dpi);
        return true;
    }

    void OpenGLFreeType::initCharacterVAO() {
        // Define quad vertices for a single character
        float quadVertices[] = {
            // positions       // texture coords
            0.0f, 1.0f,        0.0f, 1.0f,
            1.0f, 1.0f,        1.0f, 1.0f,
            1.0f, 0.0f,        1.0f, 0.0f,
            0.0f, 0.0f,        0.0f, 0.0f
        };

        // Set up VAO and VBO for character rendering
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        // Position attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

        // Texture coordinate attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        glBindVertexArray(0);
    }

    unsigned int OpenGLFreeType::loadCharacter(unsigned int ch) {
        // Load glyph for character
        FT_UInt char_index = FT_Get_Char_Index(face, ch);
        if (FT_Load_Glyph(face, char_index, FT_LOAD_RENDER) != 0) {
            std::cerr << "Error: can't load glyph for char with index.\n";
            return 0;
        }

        FT_GlyphSlot glyph = face->glyph;

        // Create texture for the glyph bitmap
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, glyph->bitmap.width, glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        textures[ch] = texture; // Store texture ID for character

        return glyph->advance.x >> 6; // Return the advance width for character spacing
    }

    void OpenGLFreeType::renderCharacter(GLuint textureID, float x, float y, float width, float height) {
        // Bind texture
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Set transformation matrix for character
        Matrix4x4 model = Matrix4x4::Translate(Matrix4x4::Identity, Vector3(x, y, 0.0f));
        model = Matrix4x4::Scale(model, Vector3(width, height, 1.0f));
        //shader.SetUniform_mat4("u_model", model);

        // Bind VAO and draw quad
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);
    }

    void OpenGLFreeType::drawText(const std::wstring& text, float x, float y, float scale) {
        float startX = x;

        // Loop through each character
        for (const wchar_t& ch : text) {
            if (ch == L'\n') {
                y += height * scale;
                x = startX;
                continue;
            }

            GLuint textureID = textures[ch];
            float width = float(chars_widths[ch]) * scale;
            float height = float(face->glyph->bitmap.rows) * scale;

            renderCharacter(textureID, x, y, width, height);

            x += width; // Move to next character position
        }
    }

    OpenGLFreeType::~OpenGLFreeType() {
        // Delete textures and buffers
        for (auto& tex : textures) glDeleteTextures(1, &tex);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }
    #endif

    void OpenGLFreeType::Init() 
    {
        #if 0
        {

        // const std::string& fontPath, unsigned int fontSize
        
        // Initialize FreeType
        if (FT_Init_FreeType(&library)) {
            Log::Fatal("Unable to initialize FreeType Library, check dll files.");
        }

        // Load all fonts in font database
        if (FT_New_Face(library, fontPath.c_str(), 0, &face)) {
            Log::Fatal("Could not load font, check font directory");
        }

        // Set font size
        FT_Set_Pixel_Sizes(face, 0, fontSize);

        // Load characters
        LoadCharacters();

        // Setup OpenGL buffers
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        }
        #endif
    }

    //void OpenGLFreeType::LoadCharacters() {
    //    for (unsigned char c = 0; c < 128; c++) {
    //        // Load character glyph
    //        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
    //            std::cerr << "Failed to load Glyph: " << c << std::endl;
    //            continue;
    //        }

    //        // Generate texture
    //        GLuint texture;
    //        glGenTextures(1, &texture);
    //        glBindTexture(GL_TEXTURE_2D, texture);
    //        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

    //        // Set texture options
    //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //        // Store character data
    //        Character character = {
    //            texture,
    //            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
    //            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
    //            static_cast<GLuint>(face->glyph->advance.x)
    //        };
    //        characters.insert(std::pair<char, Character>(c, character));
    //    }
    //}
}

#if 0 
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <string>

struct Character {
    GLuint textureID;   // Texture ID
    glm::ivec2 size;    // Size of glyph
    glm::ivec2 bearing; // Offset from baseline to left/top of glyph
    GLuint advance;     // Horizontal offset to advance to next glyph
};

class TextRenderer {
public:
    TextRenderer(const std::string& fontPath, unsigned int fontSize);
    ~TextRenderer();

    void LoadCharacters();
    void RenderText(const std::string& text, float x, float y, float scale, const glm::vec3& color);

private:
    std::map<char, Character> characters;
    GLuint VAO, VBO;
    FT_Library ft;
    FT_Face face;
    GLuint shaderProgram; // Shader program for rendering text
};

TextRenderer::TextRenderer(const std::string& fontPath, unsigned int fontSize) {
    // Initialize FreeType
    if (FT_Init_FreeType(&ft)) {
        throw std::runtime_error("Could not initialize FreeType Library");
    }

    // Load font face
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        throw std::runtime_error("Could not load font");
    }

    // Set font size
    FT_Set_Pixel_Sizes(face, 0, fontSize);

    // Load characters
    LoadCharacters();

    // Setup OpenGL buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

TextRenderer::~TextRenderer() {
    // Cleanup FreeType
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // Cleanup OpenGL resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void TextRenderer::LoadCharacters() {
    for (unsigned char c = 0; c < 128; c++) {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "Failed to load Glyph: " << c << std::endl;
            continue;
        }

        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Store character data
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
        characters.insert(std::pair<char, Character>(c, character));
    }
}

void TextRenderer::RenderText(const std::string& text, float x, float y, float scale, const glm::vec3& color) {
    // Activate shader program
    glUseProgram(shaderProgram);

    // Set text color
    glUniform3f(glGetUniformLocation(shaderProgram, "textColor"), color.x, color.y, color.z);

    // Set transformation matrix
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(x, y, 0.0f));
    transform = glm::scale(transform, glm::vec3(scale, scale, 1.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, glm::value_ptr(transform));

    // Render text
    glBindVertexArray(VAO);
    for (char c : text) {
        Character ch = characters[c];

        // Update VBO for each character
        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        float vertices[6][4] = {
            { xpos,     ypos + h, 0.0f, 0.0f },
            { xpos,     ypos,     0.0f, 1.0f },
            { xpos + w, ypos,     1.0f, 1.0f },

            { xpos,     ypos + h, 0.0f, 0.0f },
            { xpos + w, ypos,     1.0f, 1.0f },
            { xpos + w, ypos + h, 1.0f, 0.0f }
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

#endif
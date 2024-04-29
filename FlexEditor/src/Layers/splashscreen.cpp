#include "splashscreen.h"

namespace FlexEditor
{

  void SplashScreenLayer::OnAttach()
  {
    FLX_FLOW_BEGINSCOPE();

    // load shaders
    shader_texture.Create(Path::current_path("assets/shaders/texture.vert"), Path::current_path("assets/shaders/texture.frag"));

    // load image
    //img_splash.Load(); // default texture
    img_splash.Load("assets/images/flexengine/flexengine_splash.png");


    // todo: add opengl rendering
    // vertex buffer objects (VBO)
    // indices (IBO)
    // vertex array object (VAO)

    float vertices[] = {
      // positions          // colors           // texture coords
       1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // top right
       1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // bottom right
      -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // bottom left
      -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f    // top left 
    };

    unsigned int indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    vertex_buffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
    index_buffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // texture coord attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  }

  void SplashScreenLayer::OnDetach()
  {
    shader_texture.Destroy();

    FLX_FLOW_ENDSCOPE();
  }

  void SplashScreenLayer::Update()
  {
    img_splash.Bind();
    shader_texture.Use();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, index_buffer->GetCount(), GL_UNSIGNED_INT, 0);
  }

}

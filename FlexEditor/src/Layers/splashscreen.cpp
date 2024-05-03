#include "splashscreen.h"

namespace FlexEditor
{

  void SplashScreenLayer::OnAttach()
  {
    FLX_FLOW_BEGINSCOPE();

    // load shaders
    shader_texture.Create(Path::current_path("assets/shaders/texture.vert"), Path::current_path("assets/shaders/texture.frag"));

    // load image
    File& file_splash = File::Open(Path::current_path("assets/images/flexengine/flexengine_splash.png"));
    img_splash.Load(file_splash);

    // manually build a mesh
    std::vector<Vertex> vertices = {
         // positions              // colors               // texture coords
      { {  1.0f,  1.0f, 0.0f },   { 1.0f, 0.0f, 0.0f },   { 1.0f, 0.0f } },  // top right
      { {  1.0f, -1.0f, 0.0f },   { 0.0f, 1.0f, 0.0f },   { 1.0f, 1.0f } },  // bottom right
      { { -1.0f, -1.0f, 0.0f },   { 0.0f, 0.0f, 1.0f },   { 0.0f, 1.0f } },  // bottom left
      { { -1.0f,  1.0f, 0.0f },   { 1.0f, 1.0f, 0.0f },   { 0.0f, 0.0f } }   // top left 
    };

    std::vector<unsigned int> indices = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
    };

    std::vector<Asset::Texture> textures = { img_splash };

    mesh_splash = { vertices, indices, textures };
  }

  void SplashScreenLayer::OnDetach()
  {
    shader_texture.Destroy();

    FLX_FLOW_ENDSCOPE();
  }

  void SplashScreenLayer::Update()
  {
    //img_splash.Bind();
    //shader_texture.Use();
    
    //glBindVertexArray(VAO);
    //glDrawElements(GL_TRIANGLES, index_buffer->GetCount(), GL_UNSIGNED_INT, 0);

    mesh_splash.Draw(shader_texture);
  }

}

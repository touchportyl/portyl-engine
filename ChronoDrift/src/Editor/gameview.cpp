#include "gameview.h"
#include "editorgui.h"
#include "imguipayloads.h"
#include <FlexEngine/Renderer/OpenGL/openglspriterenderer.h>
#include "windowsizes.h"

namespace ChronoDrift
{
	constexpr float TOP_PADDING = 10.0f;

	void GameView::Init()
	{
	}

	void GameView::Update()
	{

	}
	void GameView::Shutdown()
	{
	}


	void GameView::EditorUI()
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
		SETGAMEWINDOW
		ImGui::Begin("Game", nullptr, window_flags);
		{
			ImVec2 window_top_left = ImGui::GetWindowPos();
			ImVec2 mouse_pos_ss = ImGui::GetMousePos(); // Screen space mouse pos

			//Panel = entire imgui window
			//Content = exclude title bar
			//Viewport = the image 
			float title_bar_height = ImGui::GetCurrentWindow()->TitleBarHeight();
			ImVec2 panel_size = ImGui::GetWindowSize();
			ImVec2 content_size = { panel_size.x, panel_size.y - title_bar_height };

			//app width and height
			float width = static_cast<float>(FlexEngine::Application::GetCurrentWindow()->GetWidth());
			float height = static_cast<float>(FlexEngine::Application::GetCurrentWindow()->GetHeight());
			float aspect_ratio = width / height;

			//also have a tiny bit of extra padding to ensure the whole image is in frame
			if (height > content_size.y - TOP_PADDING)
			{
				height = content_size.y - TOP_PADDING;
				width = height * aspect_ratio;
			}
			ImVec2 viewport_size(width, height);
			ImVec2 viewport_position((panel_size.x - viewport_size.x) / 2.0f, title_bar_height + TOP_PADDING / 2.0f);

			ImGui::SetCursorPos(viewport_position);
			//Note: need to invert UVs vertically.
			ImGui::Image((ImTextureID)static_cast<uintptr_t>(FlexEngine::OpenGLFrameBuffer::GetCreatedTexture(FlexEngine::OpenGLFrameBuffer::CreatedTextureID::CID_finalRender)),
				viewport_size, ImVec2(0, 1), ImVec2(1, 0));
    
			
			// Obtain relative mouse position in the editor view
			ImVec2 mouse_pos_imgui_viewport = ImVec2(mouse_pos_ss.x - window_top_left.x - viewport_position.x, 
																							 mouse_pos_ss.y - window_top_left.y - viewport_position.y); // IMGUI space is screen space - top left of imgui window


      // Scale coordinates by window size. Originally it was rendered with the game as the target, but then superimposed as a texture onto the imgui window.
      unsigned scale_x = Application::GetCurrentWindow()->GetWidth();
			unsigned scale_y = Application::GetCurrentWindow()->GetHeight();

      // Normalize imgui to (0,1) space, then scale by window size, which converts to world as it is a 1 to 1 mapping.
      int imgui_to_window_x = static_cast<int>(mouse_pos_imgui_viewport.x / viewport_size.x * static_cast<int>(scale_x));
      int imgui_to_window_y = static_cast<int>(mouse_pos_imgui_viewport.y / viewport_size.y * static_cast<int>(scale_y));

      mouse_to_world = FlexEngine::Vector2((float)imgui_to_window_x, (float)imgui_to_window_y);
		}

		ImGui::End();
	}
}
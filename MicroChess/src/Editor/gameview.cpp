#include "gameview.h"
#include <FlexEngine/Renderer/OpenGL/openglspriterenderer.h>

namespace ChronoShift
{
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
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollWithMouse;
		ImGui::Begin("Game", nullptr, window_flags);
		{
			ImVec2 panel_size = ImGui::GetWindowSize();
			float width = FlexEngine::Application::GetCurrentWindow()->GetWidth();
			float height = FlexEngine::Application::GetCurrentWindow()->GetHeight();

			if (height > panel_size.y)
			{
				float aspect_ratio = width / height;
				height = panel_size.y;
				width = height * aspect_ratio;
			}
			ImVec2 viewport_size(width, height);
			ImVec2 viewport_position((panel_size.x - viewport_size.x) / 2.0f, (panel_size.y - viewport_size.y) / 2.0f);

			ImGui::SetCursorPos(viewport_position);
			//Note: need to invert UVs vertically.
			ImGui::Image((ImTextureID)FlexEngine::OpenGLSpriteRenderer::GetCreatedTexture(FlexEngine::OpenGLSpriteRenderer::CreatedTextureID::CID_editor),
				viewport_size, ImVec2(0, 1), ImVec2(1, 0));
		}
		ImGui::End();
	}
}
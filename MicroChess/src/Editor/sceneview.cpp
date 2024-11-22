#include "sceneview.h"
#include "editorgui.h"
#include "imguipayloads.h"
#include <FlexEngine/Renderer/OpenGL/openglspriterenderer.h>


namespace ChronoShift
{
	constexpr float TOP_PADDING = 10.0f;

	void SceneView::Init()
	{
	}

	void SceneView::Update()
	{

	}

	void SceneView::Shutdown()
	{
	}

	void SceneView::CalculatePositions()
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

		m_viewport_size = { width, height };
		m_viewport_position = { (panel_size.x - m_viewport_size.x) / 2.0f, title_bar_height + TOP_PADDING / 2.0f }; // relative to imgui window


		// Get Mouse position relative to the viewport
		ImVec2 mouse_pos_imgui_viewport = ImVec2(mouse_pos_ss.x - window_top_left.x - m_viewport_position.x,
																						 mouse_pos_ss.y - window_top_left.y - m_viewport_position.y); // IMGUI space is screen space - top left of imgui window

		int imgui_to_window_x = static_cast<int>(mouse_pos_imgui_viewport.x / m_viewport_size.x * static_cast<int>(Application::GetCurrentWindow()->GetWidth()));
		int imgui_to_window_y = static_cast<int>(mouse_pos_imgui_viewport.y / m_viewport_size.y * static_cast<int>(Application::GetCurrentWindow()->GetHeight()));

		mouse_to_world = { static_cast<float>(imgui_to_window_x), static_cast<float>(imgui_to_window_y) };
		const CameraData* camdata = CameraManager::GetCameraData(CameraManager::GetMainCamera());
		mouse_to_world.x += camdata->position.x;
		mouse_to_world.y += camdata->position.y;
	}


	void SceneView::EditorUI()
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollWithMouse;
		

		ImGui::Begin("Scene", nullptr, window_flags);
		{
			CalculatePositions();

			//Display Scene texture
			ImGui::SetCursorPos(m_viewport_position);
			//Note: need to invert UVs vertically.
			ImGui::Image((ImTextureID)static_cast<uintptr_t>(FlexEngine::OpenGLSpriteRenderer::GetCreatedTexture(FlexEngine::OpenGLSpriteRenderer::CreatedTextureID::CID_editor)),
				m_viewport_size, ImVec2(0, 1), ImVec2(1, 0));

			if (ImGui::IsMouseClicked(0))
			{
				std::cout << mouse_to_world.x << ", " << mouse_to_world.y << "\n";
			}

			//Create new entity when dragging an image from assets to scene
			if (auto image = EditorGUI::StartWindowPayloadReceiver<const char>(PayloadTags::IMAGE))
			{
				auto scene = FlexECS::Scene::GetActiveScene();

				std::string image_key(image);
				std::filesystem::path path = image_key;
				Vector2 position{ 500.0f, 500.0f };
				FlexECS::Entity new_entity = scene->CreateEntity(path.filename().string());
				new_entity.AddComponent<IsActive>({ true });
				new_entity.AddComponent<Position>({ {position.x, position.y} });
				new_entity.AddComponent<Rotation>({});
				new_entity.AddComponent<Scale>({ Vector2::One * 100.0f });
				new_entity.AddComponent<Transform>({});
				new_entity.AddComponent<ZIndex>({});
				new_entity.AddComponent<Sprite>({ FlexECS::Scene::GetActiveScene()->Internal_StringStorage_New(image_key) });
				new_entity.AddComponent<Shader>({ FlexECS::Scene::GetActiveScene()->Internal_StringStorage_New(R"(\shaders\texture)") });
				EditorGUI::EndPayloadReceiver();
			}

		}
		ImGui::End();
	}
}
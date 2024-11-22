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

	void SceneView::CalculateViewportPosition()
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
		float app_width = FlexEngine::Application::GetCurrentWindow()->GetWidth(), app_height = FlexEngine::Application::GetCurrentWindow()->GetHeight();

		float width = app_width;
		float height = app_height;
		float aspect_ratio = width / height;

		//also have a tiny bit of extra padding to ensure the whole image is in frame
		if (height > content_size.y - TOP_PADDING)
		{
			height = content_size.y - TOP_PADDING;
			width = height * aspect_ratio;
		}

		m_viewport_size = { width, height };
		m_viewport_position = { (panel_size.x - m_viewport_size.x) / 2.0f, title_bar_height + TOP_PADDING / 2.0f }; // relative to imgui window
	}

	Vector4 SceneView::GetWorldClickPosition()
	{
		ImVec2 window_top_left = ImGui::GetWindowPos();
		ImVec2 mouse_pos_ss = ImGui::GetMousePos(); // Screen space mouse pos
		float app_width = FlexEngine::Application::GetCurrentWindow()->GetWidth(), app_height = FlexEngine::Application::GetCurrentWindow()->GetHeight();

		// Get Mouse position relative to the viewport
		ImVec2 relative_pos = ImVec2(mouse_pos_ss.x - window_top_left.x - m_viewport_position.x,
																 mouse_pos_ss.y - window_top_left.y - m_viewport_position.y); // IMGUI space is screen space - top left of imgui window


		//normalize 0, 1 coords relative to viewport, then scale by app height
		//This is mouse relative and scaled to "game" screen 
		Vector2 screen_pos = { (relative_pos.x / m_viewport_size.x) * app_width,
													 (relative_pos.y / m_viewport_size.y) * app_height };

		const CameraData* camdata = CameraManager::GetCameraData(CameraManager::GetMainCamera());

		Vector2 ndc_click_pos = { (2 * screen_pos.x / app_width) - 1, 1 - 2 * screen_pos.y / app_height };
		Matrix4x4 inverse = (camdata->proj_viewMatrix).Inverse();
		Vector4 clip = { ndc_click_pos.x,
										 ndc_click_pos.y,
										 1.0f,
										 1 };
		Vector4 world_pos = inverse * clip;
		world_pos.x = -world_pos.x;

		//std::cout << "World Pos: " << world_pos << "\n";
		return world_pos;
	}

	FlexECS::Entity SceneView::FindClickedEntity()
	{
		FlexECS::Entity clicked_entity = FlexECS::Entity::Null;
		int selected_z_index = INT_MIN;
		Vector4 mouse_world_pos = GetWorldClickPosition();

		//AABB tiem
		auto scene = FlexECS::Scene::GetActiveScene();
		for (auto entity : scene->CachedQuery<Position, Scale, Transform, Shader, ZIndex>()) //you probably only wanna click on things that are rendered
		{
			auto& pos = entity.GetComponent<Position>()->position;
			auto& scale = entity.GetComponent<Scale>()->scale;
			if (mouse_world_pos.x >= (pos.x - (scale.x / 2)) &&
					mouse_world_pos.x <= (pos.x + (scale.x / 2)) &&
					mouse_world_pos.y >= (pos.y - (scale.y / 2)) &&
					mouse_world_pos.y <= (pos.y + (scale.y / 2)))
			{
				if (entity.GetComponent<ZIndex>()->z > selected_z_index)
				{
					clicked_entity = entity;
					selected_z_index = entity.GetComponent<ZIndex>()->z;
				}
			}
		}

		return clicked_entity;
	}

	void SceneView::CheckMouseEvents()
	{
		if (ImGui::IsMouseClicked(0))
		{
			FlexECS::Entity entity = FindClickedEntity();
			if (entity != FlexECS::Entity::Null)
			{
				Editor::GetInstance().SelectEntity(entity);
			}
		}
	}


	void SceneView::EditorUI()
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollWithMouse;


		ImGui::Begin("Scene", nullptr, window_flags);
		{
			CalculateViewportPosition();
			CheckMouseEvents();

			//Display Scene texture
			ImGui::SetCursorPos(m_viewport_position);
			ImGui::Image((ImTextureID)static_cast<uintptr_t>(FlexEngine::OpenGLSpriteRenderer::GetCreatedTexture(FlexEngine::OpenGLSpriteRenderer::CreatedTextureID::CID_editor)),
				m_viewport_size, ImVec2(0, 1), ImVec2(1, 0));


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

//Vector2 relPos = { ImGui::GetMousePos().x, ImGui::GetMousePos().y };
//Vector2 minPos = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y };
//relPos -= minPos;
//std::cout << relPos.x << ", " << relPos.y << "\n";
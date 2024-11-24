#include "sceneview.h"
#include "editorgui.h"
#include "imguipayloads.h"
#include <FlexEngine/Renderer/OpenGL/openglspriterenderer.h>


namespace ChronoDrift
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
		float app_width = static_cast<float>(FlexEngine::Application::GetCurrentWindow()->GetWidth());
		float app_height = static_cast<float>(FlexEngine::Application::GetCurrentWindow()->GetHeight());

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
		float app_width = static_cast<float>(FlexEngine::Application::GetCurrentWindow()->GetWidth()); 
		float app_height = static_cast<float>(FlexEngine::Application::GetCurrentWindow()->GetHeight());


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

		return world_pos;
	}

	ImVec2 SceneView::WorldToScreen(const FlexEngine::Vector2& position)
	{
		const CameraData* camdata = CameraManager::GetCameraData(CameraManager::GetMainCamera());
		Vector4 world_pos = { -position.x, position.y, 0.0f, 1.0f };
		Vector4 clip = camdata->proj_viewMatrix * world_pos;
		if (clip.w != 0.0f)
		{
			clip.x /= clip.w;
			clip.y /= clip.w;
			clip.z /= clip.w;
		}

		// Convert to coordinates relative to viewport 
		float x_screen = ((clip.x + 1.0f) * 0.5f) * m_viewport_size.x;
		float y_screen = ((1.0f - clip.y) * 0.5f) * m_viewport_size.y;

		//finally translate viewport relative position to screen relative coords
		ImVec2 screen_pos = { x_screen, y_screen };
		screen_pos.x += ImGui::GetWindowPos().x + m_viewport_position.x;
		screen_pos.y += ImGui::GetWindowPos().y + m_viewport_position.y;

		return screen_pos;
	}

	FlexECS::Entity SceneView::FindClickedEntity()
	{
		FlexECS::Entity clicked_entity = FlexECS::Entity::Null;
		int selected_z_index = INT_MIN;
		Vector4 mouse_world_pos = GetWorldClickPosition();	//Note: need to check if mouse click is within viewport

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
		if (ImGui::IsWindowFocused() && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
		{
			if (ImGui::IsMouseClicked(0))
			{
				FlexECS::Entity entity = FindClickedEntity();
				if (entity != FlexECS::Entity::Null)
				{
					Editor::GetInstance().SelectEntity(entity);
				}
				else
				{
					Editor::GetInstance().SelectEntity(FlexECS::Entity::Null);
				}
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

			//ImVec2 cursor_pos = ImGui::GetCursorPos();
			//ImGui::IsItemHovered();

			//Display Scene texture
			ImGui::SetCursorPos(m_viewport_position);
			ImGui::Image((ImTextureID)static_cast<uintptr_t>(FlexEngine::OpenGLFrameBuffer::GetCreatedTexture(FlexEngine::OpenGLFrameBuffer::CreatedTextureID::CID_editor)),
				m_viewport_size, ImVec2(0, 1), ImVec2(1, 0));

			DrawGizmos();

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

	void SceneView::DrawGizmos()
	{
		m_gizmo_hovered = false;
		FlexECS::Entity selected_entity = Editor::GetInstance().GetSelectedEntity();

		if (selected_entity == FlexECS::Entity::Null) return;

		selected_entity.GetComponent<Transform>()->is_dirty = true;
		auto& entity_transform = selected_entity.GetComponent<Transform>()->transform;
		auto& entity_position = selected_entity.GetComponent<Position>()->position;
		auto& entity_scale = selected_entity.GetComponent<Scale>()->scale;

		//Maybe pass in the projection matrix in the future
		//const CameraData* camdata = CameraManager::GetCameraData(CameraManager::GetMainCamera());

		//Find out where on the screen to draw the gizmos
		//Take entity position and convert it back to screen position
		ImVec2 gizmo_origin_pos = WorldToScreen(entity_position);
		Vector2 pos_change{};

		//Havent diagnosed the reason, but the color changing of gizmo when you hover over stops working
		//When more than 1 gizmo is drawn.
		EditorGUI::Gizmo_Right_Arrow(&pos_change.x, { gizmo_origin_pos.x, gizmo_origin_pos.y });
		EditorGUI::Gizmo_Up_Arrow(&pos_change.y, { gizmo_origin_pos.x, gizmo_origin_pos.y });
		EditorGUI::Gizmo_XY_Rect(&pos_change.x, &pos_change.y, { gizmo_origin_pos.x, gizmo_origin_pos.y });

		//Scale the change in position with relation to screen size
		//pos_change represents how much the gizmo moved in absolute screen coordinates.
		//Need to convert screen space to world space
		pos_change.x *= FlexEngine::Application::GetCurrentWindow()->GetWidth() / m_viewport_size.x;
		pos_change.y *= FlexEngine::Application::GetCurrentWindow()->GetHeight() / m_viewport_size.y;
		
		entity_position += pos_change;
	}
}
//if (ImGui::IsMouseClicked(1))
//{
//	std::cout << "Mouse Absolute Position: " << ImGui::GetMousePos().x << ", " << ImGui::GetMousePos().y << "\n";
//	std::cout << "VP_SIZE: " << m_viewport_size.x << ", " << m_viewport_size.y << "\n";
//	std::cout << "App_SIZE : " << app_width << ", " << app_height << "\n";
//}
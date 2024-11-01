#include "sceneview.h"
#include "editorgui.h"
#include "imguipayloads.h"
#include <FlexEngine/Renderer/OpenGL/openglspriterenderer.h>


namespace ChronoShift
{
	void SceneView::Init()
	{
	}

	void SceneView::Update()
	{
		WindowProps window_props = Application::GetCurrentWindow()->GetProps();
		Renderer2DProps props;
		props.window_size = { static_cast<float>(window_props.width), static_cast<float>(window_props.height) };

		FunctionQueue finalized_render_queue;



		for (auto& entity : FlexECS::Scene::GetActiveScene()->View<IsActive, ZIndex, Transform, Shader, Sprite>())
		{
			auto entity_name_component = entity.GetComponent<EntityName>();

			if (!entity.GetComponent<IsActive>()->is_active) continue;
			auto& z_index = entity.GetComponent<ZIndex>()->z;
			Matrix4x4 transform = entity.GetComponent<Transform>()->transform;
			auto& shader = FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(entity.GetComponent<Shader>()->shader);
			auto sprite = entity.GetComponent<Sprite>();

			props.shader = shader;
			props.transform = transform;
			props.texture = FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(sprite->texture);
			props.color = sprite->color;
			props.color_to_add = sprite->color_to_add;
			props.color_to_multiply = sprite->color_to_multiply;
			props.alignment = static_cast<Renderer2DProps::Alignment>(sprite->alignment);
			props.vbo_id = sprite->vbo_id;

			finalized_render_queue.Insert({ [props]() { OpenGLSpriteRenderer::DrawTexture2D(OpenGLSpriteRenderer::GetCreatedTexture(OpenGLSpriteRenderer::CID_editor),props); }, "", z_index });

		}
	}

	void SceneView::Shutdown()
	{
	}


	void SceneView::EditorUI()
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollWithMouse;
		ImGui::Begin("Scene", nullptr, window_flags);
		{
			ImVec2 panel_size = ImGui::GetWindowSize();
			float width =  static_cast<float>(FlexEngine::Application::GetCurrentWindow()->GetWidth());
			float height = static_cast<float>(FlexEngine::Application::GetCurrentWindow()->GetHeight());

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
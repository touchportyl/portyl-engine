#include "sceneview.h"
#include "editorgui.h"
#include "imguipayloads.h"
#include <FlexEngine/Renderer/OpenGL/openglspriterenderer.h>

#include "Renderer/camera2d.h"
namespace ChronoDrift
{
	void SceneView::Init()
	{
		//TODO Setup editor cam from saved data upon close of program
		m_EditorCam = std::make_unique<FlexEngine::CameraData>();
	}

	void SceneView::Update()
	{
		UpdateEditorCam();
	}

	void SceneView::Shutdown()
	{
		m_EditorCam.reset();
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
			ImGui::Image((ImTextureID)static_cast<uintptr_t>(FlexEngine::OpenGLFrameBuffer::GetCreatedTexture(FlexEngine::OpenGLFrameBuffer::CreatedTextureID::CID_editor)),
				viewport_size, ImVec2(0, 1), ImVec2(1, 0));
		}
		ImGui::End();
	}

	void SceneView::UpdateEditorCam()
	{
		//Add controls
		if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_MIDDLE)) //Use middle mouse button to drag and move
		{
			//TODO Not sure what functions to call for this @Rocky
		}

		//Update data
		Camera2D::UpdateProjectionMatrix(*m_EditorCam.get());
		Camera2D::UpdateViewMatrix(*m_EditorCam.get());

		//Update Camera Manager
		FlexECS::EntityID currEditorID = Editor::GetInstance().GetCamManager().GetEditorCamera();
		Editor::GetInstance().GetCamManager().UpdateData(currEditorID, *m_EditorCam.get());
	}
}
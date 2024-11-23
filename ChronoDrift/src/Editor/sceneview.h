/*!************************************************************************
// WLVERSE [https://wlverse.web.app]
// sceneview.h
//
// Scene Panel for the editor.
//
// AUTHORS
// [100%] Rocky Sutarius (rocky.sutarius@digipen.edu)
//   - Main Author
//
// Copyright (c) 2024 DigiPen, All rights reserved.
**************************************************************************/

#pragma once
#include "editorpanel.h"

namespace ChronoDrift
{
	class SceneView : public EditorPanel
	{
	public:
		void Init();
		void Update();
		void EditorUI();
		void Shutdown();

	private:
		void CalculateViewportPosition();	//position of viewport image and size
		void CheckMouseEvents();
		FlexEngine::FlexECS::Entity FindClickedEntity();
		FlexEngine::Vector4 GetWorldClickPosition();

		bool m_gizmo_hovered;
		bool m_dragging;

		ImVec2 m_viewport_size;
		ImVec2 m_viewport_position;
	};
}

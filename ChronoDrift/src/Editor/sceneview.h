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

#include "FlexEngine/Renderer/cameramanager.h"
namespace ChronoDrift
{
	class SceneView : public EditorPanel
	{
		std::unique_ptr<FlexEngine::CameraData> m_EditorCam;
	public:
		void Init();
		void Update();
		void EditorUI();
		void Shutdown();

		void UpdateEditorCam();
	};
}

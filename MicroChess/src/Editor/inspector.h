/*!************************************************************************
// WLVERSE [https://wlverse.web.app]
// inspector.h
//
// Inspector Panel (properties viewer) for the editor.
//
// AUTHORS
// [100%] Rocky Sutarius (rocky.sutarius@digipen.edu)
//   - Main Author
//
// Copyright (c) 2024 DigiPen, All rights reserved.
**************************************************************************/

#pragma once
#include "FlexEngine.h"
#include "editorpanel.h"


namespace ChronoShift
{

	class Inspector : public EditorPanel
	{
	public:
		void Init();
		void Update();
		void EditorUI();
		void Shutdown();
	};

}
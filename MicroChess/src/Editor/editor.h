#pragma once

#include "hierarchyview.h"
#include "FlexEngine/Core/imguiwrapper.h"
#include "FlexEngine/Renderer/DebugRenderer/debugrenderer.h"
#include "FlexEngine.h"


namespace FlexEngine
{
	class Editor
	{
	public:
		static Editor* GetInstance();
		~Editor();

		void Init();
		void Update();
		void Shutdown();
	};

}

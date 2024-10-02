#pragma once

#include "hierarchyview.h"
#include "inspector.h"
#include "editorgui.h"
#include "componentviewer.h"
#include "Components/rendering.h"

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

		void SelectEntity(FlexECS::Entity);
		FlexECS::Entity GetSelectedEntity();

	private:
		FlexECS::Entity m_selected_entity = FlexECS::Entity::Null;
		//ComponentViewRegistry m_component_registry;
	};

}

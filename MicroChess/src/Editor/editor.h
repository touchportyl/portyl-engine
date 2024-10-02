#pragma once

#include "hierarchyview.h"
#include "inspector.h"
#include "editorgui.h"
#include "componentviewer.h"
#include "Components/rendering.h"

#include "FlexEngine/Core/imguiwrapper.h"
#include "FlexEngine/Renderer/DebugRenderer/debugrenderer.h"
#include "FlexEngine.h"

namespace ChronoShift
{
	class Editor
	{
	public:
		static Editor* GetInstance();
		~Editor();

		void Init();
		void Update();
		void Shutdown();

		void SelectEntity(FlexEngine::FlexECS::Entity);
		FlexEngine::FlexECS::Entity GetSelectedEntity();

	private:
		FlexEngine::FlexECS::Entity m_selected_entity = FlexEngine::FlexECS::Entity::Null;
		//ComponentViewRegistry m_component_registry;
	};

}

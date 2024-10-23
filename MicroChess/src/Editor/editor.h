/*!************************************************************************
// WLVERSE [https://wlverse.web.app]
// editor.h
//
// This file contains the declaration of the Editor class.
//
// AUTHORS
// [100%] Rocky Sutarius (rocky.sutarius@digipen.edu)
//   - Main Author
//
// Copyright (c) 2024 DigiPen, All rights reserved.
**************************************************************************/

#pragma once

#include "Editor/hierarchyview.h"
#include "Editor/inspector.h"
#include "Editor/editorgui.h"
#include "Editor/assetbrowser.h"
#include "Editor/componentviewer.h"

#include "Components/rendering.h"

#include "FlexEngine/Core/imguiwrapper.h"
#include "FlexEngine/Renderer/DebugRenderer/debugrenderer.h"
#include "FlexEngine.h"

#include <memory>

namespace ChronoShift
{
	class Editor
	{
	public:

		/*!***************************************************************************
		* @brief
		* Getter for our singleton class.
		******************************************************************************/
		static Editor* GetInstance();

		/*!***************************************************************************
		* @brief
		* Currently nothing inside.
		******************************************************************************/
		void Init();

		/*!***************************************************************************
		* @brief
		* Update function. Just call once every frame!
		******************************************************************************/
		void Update();

		/*!***************************************************************************
		* @brief
		* Currently nothing inside.
		******************************************************************************/
		void Shutdown();

		/*!***************************************************************************
		* @brief
		* Sets the selected entity to param entity.
		* @param Entity
		* the entity to focus onto.
		******************************************************************************/
		void SelectEntity(FlexEngine::FlexECS::Entity);
		FlexEngine::FlexECS::Entity GetSelectedEntity();

	private:
		FlexEngine::FlexECS::Entity m_selected_entity = FlexEngine::FlexECS::Entity::Null;	//Which entity the inspector panel should focus on.
		AssetBrowser m_assetbrowser;

		std::vector<std::unique_ptr<EditorPanel>> m_panels;

		bool m_initialized = false;
		
		//ComponentViewRegistry m_component_registry;
	};

}

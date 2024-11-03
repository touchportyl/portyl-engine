/*!************************************************************************
// WLVERSE [https://wlverse.web.app]
// editor.cpp
//
// This file contains the implementation of the Editor class.
//
// AUTHORS
// [100%] Rocky Sutarius (rocky.sutarius@digipen.edu)
//   - Main Author
//
// Copyright (c) 2024 DigiPen, All rights reserved.
**************************************************************************/

#pragma once
#include "editor.h"


namespace ChronoShift
{
	static Editor instance;

	Editor& Editor::GetInstance()
	{
		if (!instance.m_initialized)
		{
			instance.Init();
		}

		return instance;
	}

	void Editor::Init()
	{
		if (m_initialized) return;
		m_initialized = true;

		m_panels.push_back(&m_hierarchy);
		m_panels.push_back(&m_inspector);
		m_panels.push_back(&m_assetbrowser);
		for (auto& panel : m_panels)
		{
			panel->Init();
		}
	}

	//ImGui startframe endframe already called in States::Window
	void Editor::Update()
	{
		//ImGui::ShowDemoWindow();

		EditorGUI::StartFrame();
		for (auto& panel : m_panels)
		{
			panel->Update();
		}

		for (auto& panel : m_panels)
		{
			panel->EditorUI();
		}

		EditorGUI::EndFrame();
	}


	void Editor::Shutdown()
	{
		for (auto& panel : m_panels)
		{
			panel->Shutdown();
		}

		m_panels.clear();
	}




	template<typename T>
	T& Editor::GetPanel()
	{
		for (const auto& panel : m_panels) {
			if (T* cast = dynamic_cast<T*>(panel.get())) 
			{
				return *cast;  // Return if successful cast
			}
		}
		return nullptr;
	}


	void Editor::SelectEntity(FlexECS::Entity entity)
	{
		m_selected_entity = entity;
	}

	FlexECS::Entity Editor::GetSelectedEntity()
	{
		return m_selected_entity;
	}
}
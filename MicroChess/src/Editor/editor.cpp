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
	Editor* Editor::GetInstance()
	{
		static Editor instance{};
		if (!instance.m_initialized)
		{
			instance.Init();
		}

		return &instance;
	}

	void Editor::Init()
	{
		if (m_initialized) return;
		m_initialized = true;

		m_panels.emplace_back(std::make_unique<HierarchyView>());
		m_panels.emplace_back(std::make_unique<Inspector>());
		m_panels.emplace_back(std::make_unique<AssetBrowser>());

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


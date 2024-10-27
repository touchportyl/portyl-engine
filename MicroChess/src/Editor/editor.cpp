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




	template<typename T>
	T* Editor::GetPanel()
	{
		for (const auto& panel : m_panels) {
			if (T* cast = dynamic_cast<T*>(panel.get())) 
			{
				return cast;  // Return if successful cast
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

/*
		File& prefab_file = File::Open(path);
		// Formatter to parse metadata first, then deserialize the prefab data
		FlxFmtFile formatter = FlexFormatter::Parse(prefab_file, FlxFmtFileType::Prefab);
		std::string contents = formatter.data;

		// Make it a valid JSON object in the form of an array
		contents.append("]");
		contents.insert(0, "[");

		// Passing it into rapidjson to make life easier
		Document document;
		document.Parse(contents.c_str());

		// Loop through each member
		//for (auto& member : document.GetArray())
		//{
		//	if (member.IsObject() && member["type"].IsString())
		//	{
		//		std::string component_name = member["type"].GetString();

		//	}
		//}
		*/
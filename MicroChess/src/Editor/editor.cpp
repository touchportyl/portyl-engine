#pragma once
#include "editor.h"


namespace ChronoShift
{
	Editor* Editor::GetInstance()
	{
		static bool initialized = false;
		static Editor instance{};
		if (!initialized) instance.Init();

		return &instance;
	}
	Editor::~Editor()
	{

	}
	void Editor::Init()
	{

	}

	void Editor::Update()
	{
		EditorUI::StartFrame();
		
		DisplaySceneHierarchy();
		DisplayInspector();
		
		EditorUI::EndFrame();
	}
	void Editor::Shutdown()
	{
	}

	void Editor::SelectEntity(FlexECS::Entity entity)
	{
		m_selected_entity = entity;
	}

	FlexECS::Entity Editor::GetSelectedEntity()
	{
		return m_selected_entity;
	}

	
	//ImGui startframe endframe already called in Window

}


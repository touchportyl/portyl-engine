#pragma once
#include "editor.h"


namespace FlexEngine
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
		//ImGui::ShowDemoWindow();
		DisplaySceneHierarchy();
		DisplayInspector();
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


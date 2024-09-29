#pragma once
#include "editor.h"


namespace FlexEngine
{
	Editor* FlexEngine::Editor::GetInstance()
	{
		static Editor instance{};
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


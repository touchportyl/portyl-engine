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
		ImGui::ShowDemoWindow();

		DisplaySceneHierarchy();



	}
	void Editor::Shutdown()
	{
	}

	
	//ImGui startframe endframe already called in Window

}


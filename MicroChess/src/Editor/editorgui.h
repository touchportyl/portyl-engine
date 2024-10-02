#pragma once
#include "FlexEngine.h"
#include "editor.h"

namespace ChronoShift
{
	class EditorUI
	{
	public:
		static void DragFloat2(FlexEngine::Vector2& data, std::string title = "", 
			std::string label1 = "X", std::string label2 = "Y",
			float width = 65.0f, float drag_speed = 0.1f);

		static void DragFloat3(FlexEngine::Vector3& data, std::string title = "", 
			std::string label1 = "X", std::string label2 = "Y", std::string label3 = "Z",
			float width = 65.0f, float drag_speed = 0.1f);


		static void DragInt(int& data, std::string label1 = "",
			float width = 65.0f, float drag_speed = 0.1f);

		static void EntityReference(FlexEngine::FlexECS::Entity& data, std::string label = "entity", float width = 120.0f);
	
		static void Color3(FlexEngine::Vector3& data, std::string label1 = "color", float width = 65.0f);

		static void EditableTextField(std::string& data, std::string label1 = "", float width = 150.0f);

		static void TextField(const std::string& data);

		static void StartFrame();
		static void EndFrame();

	private:
		static int PushID();
		static void PopID();

		static int m_id;
	};
}
#pragma once
#include "FlexEngine.h"
#include "editor.h"

namespace FlexEngine
{
	namespace EditorGUI
	{

		void DragFloat3(Vector3& data, std::string label1 = "", std::string label2 = "", std::string label3 = "",
			float width = 65.0f, float drag_speed = 0.1f);

		void DragFloat2(Vector2& data, std::string label1 = "", std::string label2 = "",
			float width = 65.0f, float drag_speed = 0.1f);

		void DragInt(int& data, std::string label1 = "",
			float width = 65.0f, float drag_speed = 0.1f);

		void EntityReference(FlexECS::Entity& data, std::string label = "", float width = 120.0f);
	
		void Color3(Vector3& data, std::string label1 = "", float width = 65.0f);


	}

}
#pragma once
#include "FlexEngine.h"


namespace FlexEngine
{

	void DragFloat3(Vector3& data, std::string label1 = "", std::string label2 = "", std::string label3 = "",
		float width = 65.0f, float drag_speed = 0.1f);

	void DragFloat2(Vector2& data, std::string label1 = "", std::string label2 = "",
		float width = 65.0f, float drag_speed = 0.1f);


}
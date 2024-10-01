#include "editorgui.h"
#pragma once

namespace FlexEngine 
{


	void DragFloat3(Vector3& data, std::string label1, std::string label2, std::string label3, 
		float width, float drag_speed)
	{
		ImGui::PushItemWidth(width);
		ImGui::Text(label1.c_str()); ImGui::SameLine();
		ImGui::DragFloat("##X", &data.x, drag_speed);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::Text(label2.c_str()); ImGui::SameLine();
		ImGui::PushItemWidth(width);
		ImGui::DragFloat("##Y", &data.y, drag_speed);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::Text(label3.c_str()); ImGui::SameLine();
		ImGui::PushItemWidth(width);
		ImGui::DragFloat("##Z", &data.z, drag_speed);
		ImGui::PopItemWidth();
	}

	void DragFloat2(Vector2& data, std::string label1, std::string label2, float width, float drag_speed)
	{
		ImGui::PushItemWidth(width);
		ImGui::Text(label1.c_str()); ImGui::SameLine();
		ImGui::DragFloat("##X", &data.x, drag_speed);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::Text(label2.c_str()); ImGui::SameLine();
		ImGui::PushItemWidth(width);
		ImGui::DragFloat("##Y", &data.y, drag_speed);
		ImGui::PopItemWidth();
	}
}

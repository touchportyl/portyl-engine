#include "editorgui.h"
#pragma once


namespace FlexEngine
{
	namespace EditorGUI
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

		void DragInt(int& data, std::string label, float width, float drag_speed)
		{
			ImGui::PushItemWidth(width);
			ImGui::Text(label.c_str()); ImGui::SameLine();
			ImGui::DragInt("##", &data, drag_speed);
			ImGui::PopItemWidth();
		}

		void EntityReference(FlexECS::Entity& data, std::string label, float width)
		{
			std::string entity_name = FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(*data.GetComponent<FlexEngine::FlexECS::Scene::StringIndex>());
			
			ImGui::BeginGroup();
			ImGui::Text(label.c_str()); ImGui::SameLine();
			if (ImGui::Button(entity_name.c_str()))
			{
				Editor::GetInstance()->SelectEntity(data);
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_DRAG"))
				{
					// Assuming the payload is the Entity ID (you can customize this)
					FlexECS::Entity dropped_entity = *(FlexECS::EntityID*)payload->Data;
					data = dropped_entity;
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::EndGroup();
		}

		void Color3(Vector3& data, std::string label, float width)
		{
			ImGui::ColorEdit3(label.c_str(), data.data);
		}

	}
}

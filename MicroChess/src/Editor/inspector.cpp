#include "editor.h"
#include "hierarchyview.h"
#include "inspector.h"
#include "Components/rendering.h"

using namespace ChronoShift;
using EntityName = FlexEngine::FlexECS::Scene::StringIndex;

namespace FlexEngine
{

	void DisplayInspector()
	{
		ImGui::Begin("Inspector");
		
		auto scene = FlexECS::Scene::GetActiveScene();
		auto entity = Editor::GetInstance()->GetSelectedEntity();


		if (entity != FlexECS::Entity::Null)
		{
			float field_width = 65.0f;
			//Do mandatory components
			//IsActive and EntityName
			auto& is_active = entity.GetComponent<IsActive>()->is_active;
			ImGui::Checkbox("##IsActive", &is_active);
			ImGui::SameLine();
			std::string& name = scene->Internal_StringStorage_Get(*entity.GetComponent<EntityName>());
			
			ImGui::Text(name.c_str());
			//char text_buffer[128];
			//strncpy_s(text_buffer, name.c_str(), sizeof(text_buffer));
			//text_buffer[sizeof(text_buffer) - 1] = '\0';

			//if (ImGui::InputText("##EntityName", text_buffer, IM_ARRAYSIZE(text_buffer)))
			//{
			//	name = std::string(text_buffer);
			//}

			auto& position = entity.GetComponent<Position>()->position;
			auto& rotation = entity.GetComponent<Rotation>()->rotation;
			auto& scale = entity.GetComponent<Scale>()->scale;

			//Tranform
			if (ImGui::CollapsingHeader("Position"))
			{
				EditorGUI::DragFloat2(position, "X", "Y");
			}
			if (ImGui::CollapsingHeader("Rotation"))
			{
				EditorGUI::DragFloat3(rotation, "X", "Y", "Z");
			}
			if (ImGui::CollapsingHeader("Scale"))
			{
				EditorGUI::DragFloat2(scale, "X", "Y");
			}

			auto entity_record = ENTITY_INDEX[entity];
			auto archetype = entity_record.archetype;
			auto component_list = archetype->type;

			for (auto component_id : component_list)
			{
				auto type_descriptor = TYPE_DESCRIPTOR_LOOKUP[component_id];
				std::string component_name = type_descriptor->ToString();
				if (component_name == "Position" || component_name == "Rotation" || component_name == "Scale")
					continue;

				if (ComponentViewRegistry::ViewerExists(component_name))
				{
					if (ImGui::CollapsingHeader(component_name.c_str()))
					{
						ComponentViewRegistry::ViewComponent(component_name, entity);
					}
				}
			}
		}


		ImGui::End();
	}
}

/*

			float button_width = ImGui::CalcTextSize("...").x + ImGui::GetStyle().FramePadding.x * 2.0f;
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - button_width);
			if (ImGui::Button("..."))
			{
				ImGui::OpenPopup("ComponentOptions_Position");  // Open the popup
			}
			if (ImGui::BeginPopup("ComponentOptions_Position"))
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					// Logic to remove the component goes here
					entity.RemoveComponent<Position>();
				}
				ImGui::EndPopup();
			}
*/


/*
if (ImGui::CollapsingHeader("Transform"))
			{
				float field_width = 65.0f;
				//ImGui::Text("Transform");
				ImGui::Text("Position");
				ImGui::Text("X"); ImGui::SameLine();
				ImGui::PushItemWidth(field_width);
				ImGui::DragFloat("##posX", &position.position.x, 0.1f);
				ImGui::PopItemWidth();
				ImGui::SameLine();
				ImGui::Text("Y"); ImGui::SameLine();
				ImGui::PushItemWidth(field_width);
				ImGui::DragFloat("##posY", &position.position.y, 0.1f);
				ImGui::PopItemWidth();

				ImGui::Text("Rotation");
				ImGui::Text("X"); ImGui::SameLine();
				ImGui::PushItemWidth(field_width);
				ImGui::DragFloat("##rotX", &rotation.rotation.x, 0.1f);
				ImGui::PopItemWidth();
				ImGui::SameLine();
				ImGui::Text("Y"); ImGui::SameLine();
				ImGui::PushItemWidth(field_width);
				ImGui::DragFloat("##rotY", &rotation.rotation.y, 0.1f);
				ImGui::PopItemWidth();
				ImGui::SameLine();
				ImGui::Text("Z"); ImGui::SameLine();
				ImGui::PushItemWidth(field_width);
				ImGui::DragFloat("##rotZ", &rotation.rotation.z, 0.1f);
				ImGui::PopItemWidth();

				ImGui::Text("Scale");
				ImGui::Text("X"); ImGui::SameLine();
				ImGui::PushItemWidth(field_width);
				ImGui::DragFloat("##scaleX", &scale.scale.x, 0.1f);
				ImGui::PopItemWidth();
				ImGui::SameLine();
				ImGui::Text("Y"); ImGui::SameLine();
				ImGui::PushItemWidth(field_width);
				ImGui::DragFloat("##scaleY", &scale.scale.y, 0.1f);
				ImGui::PopItemWidth();
			}
*/
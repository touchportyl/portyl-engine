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
			std::string name = scene->Internal_StringStorage_Get(*entity.GetComponent<EntityName>());
			ImGui::Text(name.c_str());

			auto& position = entity.GetComponent<Position>()->position;
			auto& rotation = entity.GetComponent<Rotation>()->rotation;
			auto& scale = entity.GetComponent<Scale>()->scale;

			//Tranform
			if (ImGui::CollapsingHeader("Position"))
			{
				float field_width = 65.0f;
				ImGui::Text("X"); ImGui::SameLine();
				ImGui::PushItemWidth(field_width);
				ImGui::DragFloat("##posX", &position.x, 0.1f);
				ImGui::PopItemWidth();
				ImGui::SameLine();
				ImGui::Text("Y"); ImGui::SameLine();
				ImGui::PushItemWidth(field_width);
				ImGui::DragFloat("##posY", &position.y, 0.1f);
				ImGui::PopItemWidth();
			}
			if (ImGui::CollapsingHeader("Rotation"))
			{
				ImGui::Text("X"); ImGui::SameLine();
				ImGui::PushItemWidth(field_width);
				ImGui::DragFloat("##rotX", &rotation.x, 0.1f);
				ImGui::PopItemWidth();
				ImGui::SameLine();
				ImGui::Text("Y"); ImGui::SameLine();
				ImGui::PushItemWidth(field_width);
				ImGui::DragFloat("##rotY", &rotation.y, 0.1f);
				ImGui::PopItemWidth();
				ImGui::SameLine();
				ImGui::Text("Z"); ImGui::SameLine();
				ImGui::PushItemWidth(field_width);
				ImGui::DragFloat("##rotZ", &rotation.z, 0.1f);
				ImGui::PopItemWidth();
			}
			if (ImGui::CollapsingHeader("Scale"))
			{
				ImGui::Text("X"); ImGui::SameLine();
				ImGui::PushItemWidth(field_width);
				ImGui::DragFloat("##scaleX", &scale.x, 0.1f);
				ImGui::PopItemWidth();
				ImGui::SameLine();
				ImGui::Text("Y"); ImGui::SameLine();
				ImGui::PushItemWidth(field_width);
				ImGui::DragFloat("##scaleY", &scale.y, 0.1f);
				ImGui::PopItemWidth();
			}

			auto entity_record = ENTITY_INDEX[entity];
			auto archetype = entity_record.archetype;
			auto component_list = archetype->type;

			for (auto component_id : component_list)
			{
				auto type_descriptor = TYPE_DESCRIPTOR_LOOKUP[component_id];
				if (ImGui::CollapsingHeader(type_descriptor->ToString().c_str()))
				{
					FlexECS::ArchetypeMap &archetype_map = COMPONENT_INDEX[component_id];
					FlexECS::ArchetypeRecord &archetype_record = archetype_map[archetype->id];
					FlexECS::ComponentData<void> component_data = archetype->archetype_table[archetype_record.column][entity_record.row];
					void* data = FlexECS::Internal_GetComponentData(component_data).second;
					//T* out_component = reinterpret_cast<T*>(data);
				}
			}
		}


		ImGui::End();
	}
}


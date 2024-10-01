#include "editor.h"
#include "hierarchyview.h"
#include "Components/rendering.h"

using namespace ChronoShift;
using namespace FlexEngine::FlexECS;
using EntityName = FlexEngine::FlexECS::Scene::StringIndex;

namespace FlexEngine
{
	void DisplaySceneHierarchy()
	{
		auto scene = FlexECS::Scene::GetActiveScene();
		size_t entity_count = scene->entity_index.size();
		std::string entity_count_text = "Entity Count:  " + std::to_string(entity_count);
		
		FunctionQueue delete_queue;

		FlexECS::Entity entity_to_delete = FlexECS::Entity::Null;

		ImGui::Begin("Scene Hierarchy");
		ImGui::Text(entity_count_text.c_str());

		//Track Clicks when not inside tree node
		//Deselect focused entity
		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0))
		{
			Editor::GetInstance()->SelectEntity(FlexECS::Entity::Null);  // Deselect when clicking in empty space
		}
		
		//Right click menu (create entity)
		if (ImGui::IsWindowHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("RightClickMenu");
		}
		if (ImGui::BeginPopup("RightClickMenu"))
		{
			if (ImGui::MenuItem("Create Entity"))
			{
				//Add default components
				auto new_entity = Scene::CreateEntity();
				new_entity.AddComponent<IsActive>({});
				new_entity.AddComponent<Position>({});
				new_entity.AddComponent<Rotation>({});
				new_entity.AddComponent<Scale>({});
			}
			ImGui::EndPopup();
		}





		int imgui_id = 0;
		for (auto& [id, record] : scene->entity_index)
		{
			ImGui::PushID(imgui_id++);
			Entity entity(id);
			std::string name = FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(*entity.GetComponent<EntityName>());
			
			ImGuiTreeNodeFlags node_flags =
				ImGuiTreeNodeFlags_DefaultOpen |
				ImGuiTreeNodeFlags_FramePadding |
				ImGuiTreeNodeFlags_OpenOnArrow |
				ImGuiTreeNodeFlags_SpanAvailWidth;

			bool is_selected = (entity == Editor::GetInstance()->GetSelectedEntity());
			if (is_selected)
			{
				node_flags |= ImGuiTreeNodeFlags_Selected;
			}

			bool is_open = ImGui::TreeNodeEx(name.c_str(), node_flags, name.c_str());
			
			if (ImGui::IsItemClicked())
			{
				Editor::GetInstance()->SelectEntity(entity);
			}
			

			if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
				ImGui::OpenPopup("EntityOptions");
				Editor::GetInstance()->SelectEntity(entity);
			}
			if (ImGui::BeginPopup("EntityOptions"))
			{
				if (ImGui::MenuItem("Duplicate Entity")) 
				{
					scene->CloneEntity(entity);
				}
				if (ImGui::MenuItem("Destroy Entity")) 
				{
					delete_queue.Insert({ [scene, entity]() {scene->DestroyEntity(entity); }, "", 0 });
					Editor::GetInstance()->SelectEntity(FlexECS::Entity::Null);
				}
				ImGui::EndPopup();
			}

			if (is_open)
			{
				ImGui::TreePop();
			}
			ImGui::PopID();
		}


		if (ImGui::BeginPopupContextItem("EntityCreationMenu"))
		{
			if (ImGui::MenuItem("Create Entity"))
			{
				Scene::CreateEntity();
			}
			ImGui::EndPopup();
		}
		ImGui::End();

		delete_queue.Flush();
	}
}


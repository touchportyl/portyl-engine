#include "editor.h"
#include "hierarchyview.h"


using namespace FlexEngine::FlexECS;
using EntityName = FlexEngine::FlexECS::Scene::StringIndex;

namespace FlexEngine
{
	void DisplaySceneHierarchy()
	{
		auto scene = FlexECS::Scene::GetActiveScene();
		size_t entity_count = scene->entity_index.size();
		std::string entity_count_text = "Entity Count:  " + std::to_string(entity_count);

		FlexECS::Entity entity_to_delete = FlexECS::Entity::Null;

		ImGui::Begin("Scene Hierarchy");
		ImGui::Text(entity_count_text.c_str());

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
			
			if (ImGui::BeginPopupContextItem("EntityContextMenu"))  // Begin right-click context menu
			{
				if (ImGui::MenuItem("Delete Entity"))
				{
					entity_to_delete = entity;
				}

				if (ImGui::MenuItem("Duplicate Entity"))
				{
					// Handle entity duplication here
					scene->CloneEntity(entity);
				}

				ImGui::EndPopup();  // End context menu
			}

			if (is_open)
			{
				ImGui::TreePop();
			}
			ImGui::PopID();
		}

		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0))
		{
			Editor::GetInstance()->SelectEntity(FlexECS::Entity::Null);  // Deselect when clicking in empty space
		}

		ImGui::End();


		if (entity_to_delete != FlexECS::Entity::Null)
		{
			scene->DestroyEntity(entity_to_delete);
		}

	}
}


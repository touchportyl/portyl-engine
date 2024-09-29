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

		ImGui::Begin("Scene Hierarchy");
		ImGui::Text(entity_count_text.c_str());

		int i = 0;
		for (auto& [id, record] : scene->entity_index)
		{
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
			
			if (is_open)
			{
				ImGui::TreePop();
			}
			++i;
		}

		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0))
		{
			Editor::GetInstance()->SelectEntity(FlexECS::Entity::Null);  // Deselect when clicking in empty space
		}

		ImGui::End();




	}
}


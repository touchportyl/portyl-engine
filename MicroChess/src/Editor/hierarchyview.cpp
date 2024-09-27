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

		for (auto& [id, record] : scene->entity_index)
		{
			Entity entity(id);
			std::string name = FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(*entity.GetComponent<EntityName>());
			ImGui::Text(name.c_str());

		}


		ImGui::End();




	}
}


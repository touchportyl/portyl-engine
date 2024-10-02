#pragma once
#include "FlexEngine.h"
#include "editorgui.h"

namespace ChronoShift
{
	//Each component should provide it's own display functionality for the editor
	using ComponentViewer = std::function<void(FlexEngine::FlexECS::Entity)>;

	class ComponentViewRegistry
	{
	public:
		
		static void RegisterComponent(std::string, ComponentViewer);
		
		static bool ViewerExists(std::string);

		static void ViewComponent(std::string, FlexEngine::FlexECS::Entity);

	private:
		static std::unordered_map<std::string, ComponentViewer> m_component_viewer;

	};





	#define COMPONENT_VIEWER_START(TYPE) \
void COMPONENT_VIEWER_##TYPE(FlexEngine::FlexECS::Entity entity) \
{ \
  using T = TYPE;

	#define COMPONENT_VIEWER_DRAG_FLOAT2(name) \
  EditorUI::DragFloat2(entity.GetComponent<T>()->name, #name, "X", "Y");

	#define COMPONENT_VIEWER_DRAG_FLOAT3(name) \
  EditorUI::DragFloat3(entity.GetComponent<T>()->name, #name, "X", "Y", "Z"); 

	#define COMPONENT_VIEWER_DRAG_INT(name) \
  EditorUI::DragInt(entity.GetComponent<T>()->name, #name); 

	#define COMPONENT_VIEWER_ENTITY_REFERENCE(name) \
  EditorUI::EntityReference(entity.GetComponent<T>()->name, #name); 

	#define COMPONENT_VIEWER_COLOR3(name) \
  EditorUI::Color3(entity.GetComponent<T>()->name, #name); 

	#define COMPONENT_VIEWER_EDITABLE_STRING(name) \
	std::string& str = FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(*entity.GetComponent<T>()->name); \
	EditorUI::EditableTextField(str, #name);

	#define COMPONENT_VIEWER_STRING(name) \
	std::string& str = FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(*entity.GetComponent<T>()->name); \
	EditorUI::TextField(str);

	#define COMPONENT_VIEWER_END(name) \
}

	#define REGISTER_COMPONENT_VIEWER(TYPE) \
	ComponentViewRegistry::RegisterComponent(#TYPE, COMPONENT_VIEWER_##TYPE);

}
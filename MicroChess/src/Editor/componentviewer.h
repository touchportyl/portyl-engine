#pragma once
#include "FlexEngine.h"
#include "editorgui.h"

namespace FlexEngine
{

	//Each component should provide it's own display functionality
	using ComponentViewer = std::function<void(FlexEngine::FlexECS::Entity)>;

	class ComponentViewRegistry
	{
	public:

		static void RegisterComponent(std::string, ComponentViewer);
		
		static bool ViewerExists(std::string);

		static void ViewComponent(std::string, FlexECS::Entity);

	private:
		static std::unordered_map<std::string, ComponentViewer> m_component_viewer;

	};


	#define COMPONENT_VIEWER_START(TYPE) \
void COMPONENT_VIEWER_##TYPE(FlexECS::Entity entity) \
{ \
  using T = TYPE;

	#define COMPONENT_VIEWER_DRAG_FLOAT2(name) \
  EditorGUI::DragFloat2(entity.GetComponent<T>()->name, "X", "Y");

	#define COMPONENT_VIEWER_DRAG_FLOAT3(name) \
  EditorGUI::DragFloat3(entity.GetComponent<T>()->name, "X", "Y", "Z"); 

	#define COMPONENT_VIEWER_DRAG_INT(name) \
  EditorGUI::DragInt(entity.GetComponent<T>()->name, #name); 

	#define COMPONENT_VIEWER_ENTITY_REFERENCE(name) \
  EditorGUI::EntityReference(entity.GetComponent<T>()->name, #name); 

	#define COMPONENT_VIEWER_COLOR3(name) \
  EditorGUI::Color3(entity.GetComponent<T>()->name, #name); 


	#define COMPONENT_VIEWER_END() \
}

	#define REGISTER_COMPONENT_VIEWER(TYPE) \
	ComponentViewRegistry::RegisterComponent(#TYPE, COMPONENT_VIEWER_##TYPE);

}
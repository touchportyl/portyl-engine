#include "componentviewer.h"


namespace FlexEngine
{
	std::unordered_map<std::string, ComponentViewer> ComponentViewRegistry::m_component_viewer;

	void ComponentViewRegistry::RegisterComponent(std::string name, ComponentViewer display)
	{
		m_component_viewer[name] = display;
	}

	bool ComponentViewRegistry::ViewerExists(std::string name)
	{
		if (auto search = m_component_viewer.find(name); search == m_component_viewer.end())
			return false;
		else return true;
	}

	void ComponentViewRegistry::ViewComponent(std::string name, FlexECS::Entity entity)
	{
		if (auto search = m_component_viewer.find(name); search == m_component_viewer.end())
			return;

		m_component_viewer[name](entity);
	}
}
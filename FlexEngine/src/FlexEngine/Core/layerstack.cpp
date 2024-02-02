#include "pch.h"

#include "layerstack.h"

namespace FlexEngine {

  LayerStack::~LayerStack()
  {
    for (Layer* layer : m_layers)
    {
      layer->OnDetach();
      delete layer;
    }
  }

  void LayerStack::PushLayer(Layer* layer)
  {
    m_layers.emplace(m_layers.begin() + m_layers_insert_index, layer);
    m_layers_insert_index++;
  }

  void LayerStack::PushOverlay(Layer* overlay)
  {
    m_layers.emplace_back(overlay);
  }

  void LayerStack::PopLayer(Layer* layer)
  {
    auto it = std::find(m_layers.begin(), m_layers.begin() + m_layers_insert_index, layer);
    if (it != m_layers.begin() + m_layers_insert_index)
    {
      layer->OnDetach();
      m_layers.erase(it);
      m_layers_insert_index--;
    }
  }

  void LayerStack::PopOverlay(Layer* overlay)
  {
    auto it = std::find(m_layers.begin() + m_layers_insert_index, m_layers.end(), overlay);
    if (it != m_layers.end())
    {
      overlay->OnDetach();
      m_layers.erase(it);
    }
  }

}
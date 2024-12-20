#include <LayerStackEngine.hpp>

LayerStackEngine::~LayerStackEngine()
{
    for (LayerEngine* layer : m_Layers)
    {
        layer->OnDetach();
        delete layer;
    }
}

void LayerStackEngine::PushLayer(LayerEngine* layer)
{
    m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
    m_LayerInsertIndex++;
}

void LayerStackEngine::PushOverlay(LayerEngine* overlay)
{
    m_Layers.emplace_back(overlay);
}

void LayerStackEngine::PopLayer(LayerEngine* layer)
{
    auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
    if (it != m_Layers.begin() + m_LayerInsertIndex)
    {
        layer->OnDetach();
        m_Layers.erase(it);
        m_LayerInsertIndex--;
    }
}

void LayerStackEngine::PopOverlay(LayerEngine* overlay)
{
    auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
    if (it != m_Layers.end())
    {
        overlay->OnDetach();
        m_Layers.erase(it);
    }
}
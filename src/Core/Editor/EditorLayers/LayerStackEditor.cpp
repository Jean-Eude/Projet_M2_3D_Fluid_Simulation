#include <LayerStackEditor.hpp>

LayerStackEditor::~LayerStackEditor() {
    while (!m_Layers.empty()) {
        LayerEditor* layer = m_Layers.top();
        m_Layers.pop();
        if (layer) {
            layer->OnDetach();
            delete layer;
        }
    }

    while (!m_Overlays.empty()) {
        LayerEditor* overlay = m_Overlays.top();
        m_Overlays.pop();
        if (overlay) {
            overlay->OnDetach();
            delete overlay;
        }
    }
}

void LayerStackEditor::PushLayer(LayerEditor* layer) {
    if (layer) {
        m_Layers.push(layer);
        LayerEditorPushed layersEditorEvents(layer);
        gEventManager.TriggerEvent(layersEditorEvents, 0);
    }
}

void LayerStackEditor::PushOverlay(LayerEditor* overlay) {
    if (overlay) {
        m_Overlays.push(overlay);
        OverlayEditorPushed layersEditorEvents(overlay);
        gEventManager.TriggerEvent(layersEditorEvents, 0);
    }
}

void LayerStackEditor::PopLayer() {
    if (!m_Layers.empty()) {
        LayerEditor* layer = m_Layers.top();
        m_Layers.pop();
        if (layer) {
            layer->OnDetach();
            LayerEditorPoped layersEditorEvents(layer);
            gEventManager.TriggerEvent(layersEditorEvents, 0);
            delete layer;
        }
    }
}

void LayerStackEditor::PopOverlay() {
    if (!m_Overlays.empty()) {
        LayerEditor* overlay = m_Overlays.top();
        m_Overlays.pop();
        if (overlay) {
            overlay->OnDetach();
            OverlayEditorPoped layersEditorEvents(overlay);
            gEventManager.TriggerEvent(layersEditorEvents, 0);
            delete overlay;
        }
    }
}

void LayerStackEditor::InitLayersAndOverlays() {
    // Render layers
    std::stack<LayerEditor*> layersCopy = m_Layers;
    while (!layersCopy.empty()) {
        LayerEditor* layer = layersCopy.top();
        layersCopy.pop();
        if (layer) {
            layer->OnInit();
        }
    }

    // Render overlays
    std::stack<LayerEditor*> overlaysCopy = m_Overlays;
    while (!overlaysCopy.empty()) {
        LayerEditor* overlay = overlaysCopy.top();
        overlaysCopy.pop();
        if (overlay) {
            overlay->OnInit();
        }
    }
}

void LayerStackEditor::RenderLayersAndOverlays() {
    // Render layers
    std::stack<LayerEditor*> layersCopy = m_Layers;
    while (!layersCopy.empty()) {
        LayerEditor* layer = layersCopy.top();
        layersCopy.pop();
        if (layer) {
            layer->OnRender();
        }
    }

    // Render overlays
    std::stack<LayerEditor*> overlaysCopy = m_Overlays;
    while (!overlaysCopy.empty()) {
        LayerEditor* overlay = overlaysCopy.top();
        overlaysCopy.pop();
        if (overlay) {
            overlay->OnRender();
        }
    }
}
#pragma once

#include <LayerEditor.hpp>

#include <EventManager.hpp>
#include <EventHandler.hpp>
#include <LayersEvents.hpp>

#include <LogsManager.hpp>

class LayerStackEditor : public LogsManager {
    public:
        LayerStackEditor() {    Subscribe<LayerEditorPushed>(m_layerEditorPushed, 0); 
                                Subscribe<LayerEditorPoped>(m_layerEditorPoped, 0);
                                Subscribe<OverlayEditorPushed>(m_overlayEditorPushed, 0); 
                                Subscribe<OverlayEditorPoped>(m_overlayEditorPoped, 0);
                            };
        ~LayerStackEditor();

        void PushLayer(LayerEditor* layer);
        void PushOverlay(LayerEditor* overlay);
        void PopLayer();
        void PopOverlay();

        void InitLayersAndOverlays();
        void RenderLayersAndOverlays();

        std::stack<LayerEditor*> m_Layers;
        std::stack<LayerEditor*> m_Overlays;

        EventHandler<LayerEditorPushed> m_layerEditorPushed = [this](const LayerEditorPushed& e) {
            OnLayerEditorPushed(e);
        };
        EventHandler<LayerEditorPoped> m_layerEditorPoped = [this](const LayerEditorPoped& e) {
            OnLayerEditorPoped(e);
        };
        EventHandler<OverlayEditorPushed> m_overlayEditorPushed = [this](const OverlayEditorPushed& e) {
            OnOverlayEditorPushed(e);
        };
        EventHandler<OverlayEditorPoped> m_overlayEditorPoped = [this](const OverlayEditorPoped& e) {
            OnOverlayEditorPoped(e);
        };

        void OnLayerEditorPushed(const LayerEditorPushed& e) {
            LogsManager::enqueueConsoleLogsInit(e.ToString(), LogLevel::DEBUG);
        }

        void OnLayerEditorPoped(const LayerEditorPoped& e) {
            LogsManager::enqueueConsoleLogsInit(e.ToString(), LogLevel::DEBUG);
        }

        void OnOverlayEditorPushed(const OverlayEditorPushed& e) {
            LogsManager::enqueueConsoleLogsInit(e.ToString(), LogLevel::DEBUG);
        }

        void OnOverlayEditorPoped(const OverlayEditorPoped& e) {
            LogsManager::enqueueConsoleLogsInit(e.ToString(), LogLevel::DEBUG);
        }
};
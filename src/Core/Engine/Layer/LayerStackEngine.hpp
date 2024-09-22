#pragma once

#include <LayerEngine.hpp>

#include <EventManager.hpp>
#include <EventHandler.hpp>
#include <LayersEvents.hpp>

#include <LogsManager.hpp>


class LayerStackEngine : public LogsManager {
    public:
        LayerStackEngine() {    Subscribe<LayerEnginePushed>(m_layerEnginePushed, 0); 
                                Subscribe<LayerEnginePoped>(m_layerEnginePoped, 0);
                                Subscribe<OverlayEnginePushed>(m_overlayEnginePushed, 0); 
                                Subscribe<OverlayEnginePoped>(m_overlayEnginePoped, 0);
                            };
        ~LayerStackEngine();

        void PushLayer(LayerEngine* layer);
        void PushOverlay(LayerEngine* overlay);
        void PopLayer(LayerEngine* layer);
        void PopOverlay(LayerEngine* overlay);

        std::vector<LayerEngine*>::iterator begin() { return m_Layers.begin(); }
        std::vector<LayerEngine*>::iterator end() { return m_Layers.end(); }
        std::vector<LayerEngine*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
        std::vector<LayerEngine*>::reverse_iterator rend() { return m_Layers.rend(); }

        std::vector<LayerEngine*>::const_iterator begin() const { return m_Layers.begin(); }
        std::vector<LayerEngine*>::const_iterator end()	const { return m_Layers.end(); }
        std::vector<LayerEngine*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
        std::vector<LayerEngine*>::const_reverse_iterator rend() const { return m_Layers.rend(); }

    private:
        std::vector<LayerEngine*> m_Layers;
        unsigned int m_LayerInsertIndex = 0;

        // Engine Layers Events
        EventHandler<LayerEnginePushed> m_layerEnginePushed = [this](const LayerEnginePushed& e) {
            OnLayerEnginePushed(e);
        };
        EventHandler<LayerEnginePoped> m_layerEnginePoped = [this](const LayerEnginePoped& e) {
            OnLayerEnginePoped(e);
        };
        EventHandler<OverlayEnginePushed> m_overlayEnginePushed = [this](const OverlayEnginePushed& e) {
            OnOverlayEnginePushed(e);
        };
        EventHandler<OverlayEnginePoped> m_overlayEnginePoped = [this](const OverlayEnginePoped& e) {
            OnOverlayEnginePoped(e);
        };

        void OnLayerEnginePushed(const LayerEnginePushed& e) {
            LogsManager::enqueueConsoleLogsInit(e.ToString(), LogLevel::DEBUG);
        }

        void OnLayerEnginePoped(const LayerEnginePoped& e) {
            LogsManager::enqueueConsoleLogsInit(e.ToString(), LogLevel::DEBUG);
        }

        void OnOverlayEnginePushed(const OverlayEnginePushed& e) {
            LogsManager::enqueueConsoleLogsInit(e.ToString(), LogLevel::DEBUG);
        }

        void OnOverlayEnginePoped(const OverlayEnginePoped& e) {
            LogsManager::enqueueConsoleLogsInit(e.ToString(), LogLevel::DEBUG);
        }
};
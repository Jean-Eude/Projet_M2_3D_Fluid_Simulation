#pragma once

#include <EditorManager.hpp>
#include <FilePath.hpp>
#include <LogsEditorLayer.hpp>
#include <ViewportEditorLayer.hpp>
#include <ToolsEditorLayer.hpp>
#include <LayerStackEditor.hpp>
#include <ParticlesManager.hpp>

class Editor : public EditorManager {
    public:
        Editor();
        ~Editor() = default;

        void OnInitUI(GLFWwindow* window);
        void OnUpdateUI();
        void OnRenderUI();
        void OnDestroyUI();

    private:
        LayerStackEditor layerStack;
        static std::vector<LayerEditor*> CreateLayers(LayerStackEditor& stackEditor);

        void pushLayers() {
            for (LayerEditor* layer : CreateLayers(layerStack)) {
                layerStack.PushLayer(layer);
            }
        }
};
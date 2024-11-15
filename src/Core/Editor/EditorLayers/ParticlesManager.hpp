#pragma once

#include <LayerEditor.hpp>
#include <LayerStackEditor.hpp>

class ParticlesManager : public LayerEditor {
    public:
        ParticlesManager(std::string name) : LayerEditor(name) {
            m_renderFunction = [this]() {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
                ImGui::Begin("Particles Manager");

                float a = 1.0f;
                ImGui::SliderFloat("test", &a, 0.0f, 1.0f);

                ImGui::End();
                ImGui::PopStyleVar();
            };
        }

        virtual ~ParticlesManager() = default;

        void OnInit() override {
            
        }

        void OnRender() override {
            m_renderFunction();
        }

    private:
        std::function<void()> m_renderFunction;
};
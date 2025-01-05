#pragma once

#include <LayerEditor.hpp>
#include <LayerStackEditor.hpp>

#include <SharedServices.hpp>

class ParametersLayerEditor : public LayerEditor {
    public:
        ParametersLayerEditor(std::string name)
        : LayerEditor(name) {
            m_renderFunction = [this]() {
                auto col_Lum = SharedServices::GetInstance().GetService<glm::vec3>("Color_Lum");
                auto pos_Lum = SharedServices::GetInstance().GetService<glm::vec3>("Pos_Lum");

                auto albedo_PBR = SharedServices::GetInstance().GetService<glm::vec3>("Albedo_PBR");
                auto metallic_PBR = SharedServices::GetInstance().GetService<float>("Metallic_PBR");
                auto roughness_PBR = SharedServices::GetInstance().GetService<float>("Roughness_PBR");
                auto ao_PBR = SharedServices::GetInstance().GetService<float>("AO_PBR");
                auto ambient_PBR = SharedServices::GetInstance().GetService<float>("Ambient_PBR");

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
                ImGui::Begin("Paramètres");

                ImGui::Text("Paramètres - Lumière");
                ImGui::ColorEdit3("Couleur - Lumière", glm::value_ptr(*col_Lum));
                ImGui::SliderFloat3("Position - Lumière", glm::value_ptr(*pos_Lum), -50.f, 50.f);
                ImGui::Spacing();
                ImGui::Text("Paramètres - PBR");
                ImGui::ColorEdit3("Couleur - PBR", glm::value_ptr(*albedo_PBR));
                ImGui::SliderFloat("Metallic - PBR", metallic_PBR.get(), 0.f, 1.f);
                ImGui::SliderFloat("Roughness - PBR", roughness_PBR.get(), 0.f, 1.f);
                ImGui::SliderFloat("AO - PBR", ao_PBR.get(), 0.f, 1.f);
                ImGui::SliderFloat("Ambient - PBR", ambient_PBR.get(), 0.f, 1.f);

                ImGui::End();
                ImGui::PopStyleVar();

            };
        }

        virtual ~ParametersLayerEditor() = default;

        void OnInit() override {
            
        }

        void OnRender() override {
            m_renderFunction();
        }

    private:
        std::function<void()> m_renderFunction;
};
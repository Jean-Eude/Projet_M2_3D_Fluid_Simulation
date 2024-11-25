#pragma once

#include <LayerEditor.hpp>
#include <LayerStackEditor.hpp>

#include <SharedServices.hpp>

class ParticlesManager : public LayerEditor {
    public:
        ParticlesManager(std::string name)
        : LayerEditor(name) {
            m_renderFunction = [this]() {
                //auto nbParticules = SharedServices::GetInstance().GetService<int>("nbParticules");

                auto particleRestDensity = SharedServices::GetInstance().GetService<float>("particleRestDensity");
                auto mass = SharedServices::GetInstance().GetService<float>("mass");
                auto visco = SharedServices::GetInstance().GetService<float>("viscosity");
                auto stiff = SharedServices::GetInstance().GetService<float>("stiffness");
                auto size = SharedServices::GetInstance().GetService<float>("sizeParti");
                auto resti = SharedServices::GetInstance().GetService<float>("restitution");
                auto gravi = SharedServices::GetInstance().GetService<float>("gravity");
                auto gravityFollowsCamera = SharedServices::GetInstance().GetService<bool>("gravityFollowsCamera");

                auto updateFunc = SharedServices::GetInstance().GetFunction<void, float, std::vector<glm::vec3>&>("updateMesh");

                //std::cout << *resti.get() << std::endl;

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
                ImGui::Begin("Gestionnaire de particules");

                //ImGui::SliderInt("Nombre de particules", nbParticules.get(), 0.0, 15000);
                ImGui::SliderFloat("Rest Density", particleRestDensity.get(), 0.01f, 5000.f);
                ImGui::SliderFloat("Masse de la particule", mass.get(), 0.01f, 0.5f);
                ImGui::SliderFloat("Viscosité", visco.get(), 0.01f, 10.f);
                ImGui::SliderFloat("Stiffness = K", stiff.get(), 0.1f, 10.f);
                ImGui::SliderFloat("Taille Particule ", size.get(), 0.1f, 50.f);
                ImGui::SliderFloat("Coefficent de restitution ", resti.get(), 0.01f, 1.f);
                ImGui::SliderFloat("Gravité ", gravi.get(), -50.0f, 50.0f);

                ImGui::Checkbox("Gravité en fonction de la caméra", gravityFollowsCamera.get());

                if (ImGui::Button("Importer un modèle")) {
                    IGFD::FileDialogConfig config;
                    config.path = "../Assets/EngineAssets/Models";
                    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Importer un modèle", ".obj", config);
                }
                if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
                    if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
                        std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                        std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                    }
                    
                    ImGuiFileDialog::Instance()->Close();
                }

                //std::cout << *a << std::endl;

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
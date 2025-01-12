#pragma once

#include <LayerEditor.hpp>
#include <LayerStackEditor.hpp>

#include <SharedServices.hpp>

class ParticlesManager : public LayerEditor {
    public:
        ParticlesManager(std::string name)
        : LayerEditor(name) {
            m_renderFunction = [this]() {
                auto particleRestDensity = SharedServices::GetInstance().GetService<float>("particleRestDensity");
                auto mass = SharedServices::GetInstance().GetService<float>("mass");
                auto visco = SharedServices::GetInstance().GetService<float>("viscosity");
                auto stiff = SharedServices::GetInstance().GetService<float>("stiffness");
                auto size = SharedServices::GetInstance().GetService<float>("sizeParti");
                auto resti = SharedServices::GetInstance().GetService<float>("restitution");
                auto gravi = SharedServices::GetInstance().GetService<float>("gravity");
                auto gravityFollowsCamera = SharedServices::GetInstance().GetService<bool>("gravityFollowsCamera");
                auto colorParti = SharedServices::GetInstance().GetService<glm::vec3>("CouleurParticule");
                auto abso = SharedServices::GetInstance().GetService<float>("CoeffAbso");
                auto sig = SharedServices::GetInstance().GetService<float>("Sigma");
                auto hasMeshCollision = SharedServices::GetInstance().GetService<bool>("hasMeshCollision");
                auto shadingFormat = SharedServices::GetInstance().GetService<int>("shadingFormat");

                const char* items[] = {
                    "Screen-Space Rendering",
                    "Position",
                    "Vélocité"
                };
                const int numItems = IM_ARRAYSIZE(items);

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
                ImGui::Begin("Gestionnaire de particules");

                ImGui::Text("Paramètres des particules");
                ImGui::SliderFloat("Rest Density", particleRestDensity.get(), 0.01f, 5000.f);
                ImGui::SliderFloat("Masse de la particule", mass.get(), 0.01f, 0.5f);
                ImGui::SliderFloat("Viscosité", visco.get(), 0.01f, 10.f);
                ImGui::SliderFloat("Stiffness = K", stiff.get(), 0.1f, 10.f);
                ImGui::SliderFloat("Taille Particule", size.get(), 0.1f, 50.f);
                ImGui::SliderFloat("Coefficent de restitution", resti.get(), 0.01f, 1.f);
                ImGui::SliderFloat("Gravité", gravi.get(), -50.0f, 50.0f);
                ImGui::Checkbox("Gravité en fonction de la caméra", gravityFollowsCamera.get());

                ImGui::Spacing();
                ImGui::Text("Affichage des particules");
                ImGui::Combo("Méthode d'affichage", shadingFormat.get(), items, numItems);

                ImGui::Spacing();
                ImGui::Text("Paramètres du Screen Space Rendering");
                ImGui::ColorEdit3("Couleur des particules", glm::value_ptr(*colorParti));
                ImGui::SliderFloat("Coefficient d'absorption", abso.get(), 0.0f, 10.0f);
                ImGui::SliderFloat("Sigma", sig.get(), 0.0f, 1.0f);
                
                ImGui::Spacing();
                ImGui::Text("Paramètres du maillage");
                ImGui::Checkbox("Afficher le maillage", hasMeshCollision.get());

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
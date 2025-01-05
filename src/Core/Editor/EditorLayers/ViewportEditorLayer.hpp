#pragma once

#include <LayerEditor.hpp>
#include <LayerStackEditor.hpp>

#include <GPUBuffersManager.hpp>

#include <SharedServices.hpp>

class ViewportEditorLayer : public LayerEditor, public GPUBuffersManager {
    public:
        ViewportEditorLayer(std::string name) : LayerEditor(name) {
            m_renderFunction = [this]() {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
                ImGui::Begin(ICON_FA_IMAGE "  Viewport");

                auto screensize = SharedServices::GetInstance().GetService<bool>("ScreenSize");

                float window_width = ImGui::GetContentRegionAvail().x;
                float window_height = ImGui::GetContentRegionAvail().y;

                SharedServices::GetInstance().SetService<glm::vec2>("ScreenSize", glm::vec2(window_width, window_height));

                getGPUBuffersManager()->rescaleFBO("fbom", window_width, window_height);

                ImVec2 pos = ImGui::GetCursorScreenPos();
                ImGui::GetWindowDrawList()->AddImage(
                    reinterpret_cast<void*>(static_cast<uintptr_t>(getGPUBuffersManager()->getFBO_IDByName("fbom"))),
                    ImVec2(pos.x, pos.y), 
                    ImVec2(pos.x + window_width, pos.y + window_height), 
                    ImVec2(0, 1), 
                    ImVec2(1, 0)
                );
                ImGui::End();
                ImGui::PopStyleVar();
            };
        }

        virtual ~ViewportEditorLayer() = default;

        void OnInit() override {
            //m_fbo.createFBO(1280, 720, FBOType::RENDER_TO_SCREEN);
            getGPUBuffersManager()->enqueueFBO("fbom", 1280, 720, FBOType::RENDER_TO_SCREEN);
        }

        void OnRender() override {
            m_renderFunction();
        }

    private:
        std::function<void()> m_renderFunction;

        GPUBuffersManager* getGPUBuffersManager() {
            auto gpuBuffersManager = SharedServices::GetInstance().GetService<GPUBuffersManager>("fbo");
            if (!gpuBuffersManager) {
                std::cerr << "Error: GPUBuffersManager service not found for ViewportFBO." << std::endl;
            }
            return gpuBuffersManager.get();
        }
};
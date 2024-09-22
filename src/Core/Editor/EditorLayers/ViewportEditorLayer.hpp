#pragma once

#include <LayerEditor.hpp>
#include <LayerStackEditor.hpp>

#include <GPUBuffersManager.hpp>

class ViewportEditorLayer : public LayerEditor, public GPUBuffersManager {
    public:
        ViewportEditorLayer(std::string name) : LayerEditor(name) {
            m_renderFunction = [this]() {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
                ImGui::Begin(ICON_FA_IMAGE "  Viewport");
     
                float window_width = ImGui::GetContentRegionAvail().x;
                float window_height = ImGui::GetContentRegionAvail().y;

                m_fbo.rescaleFBO(window_width, window_height);

                ImVec2 pos = ImGui::GetCursorScreenPos();
                ImGui::GetWindowDrawList()->AddImage(
                    (void *)m_fbo.texid, 
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
            m_fbo.createFBO(1280, 720);
        }

        void OnRender() override {
            m_renderFunction();
        }

    private:
        std::function<void()> m_renderFunction;
};
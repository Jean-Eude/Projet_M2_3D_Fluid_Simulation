#pragma once

#include <LayerEditor.hpp>
#include <LayerStackEditor.hpp>

#include <TimerManager.hpp>

class ToolsEditorLayer : public LayerEditor, public TimerManager {
    public:
        ToolsEditorLayer(std::string name) : LayerEditor(name) {
            m_renderFunction = [this]() {
                if (ImGui::BeginMainMenuBar()) {
                    if (ImGui::BeginMenu(ICON_FA_WRENCH "  Tools")) {
                        if (ImGui::MenuItem(ICON_MD_TIMER "  Timers Management")) {
                            showWindow = true;
                        }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMainMenuBar();
                }; 

                if (showWindow) {
                    ImGui::Begin(ICON_MD_TIMER "  Timers Management", &showWindow, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
                    ImGui::Text("List of Timers");
                    for(size_t i = 0; i < m_TimersList.size(); i++) {
                        ImGui::Separator();
                        std::string text = "Name: " + m_TimersList.at(0).getName() 
                                                + "   |   FPS: " + std::to_string(m_TimersList.at(0).getFPS()) 
                                                + "   |   MS: " + std::to_string(m_TimersList.at(0).getMS()) 
                                                + "   |   Delta Time: " + std::to_string(m_TimersList.at(0).getDeltaTime()) 
                                                + "   |   Time since start (in s): " + std::to_string(m_TimersList.at(0).getTotalTimeinSeconds());

                        ImGui::Text("%s", text.c_str());

                        float buttonWidth = 20.0f;
                        float spacing = ImGui::GetStyle().ItemSpacing.x;
                        float availableWidth = ImGui::GetContentRegionAvail().x;
                        float totalButtonWidth = buttonWidth * 2 + spacing;

                        ImGui::SameLine(availableWidth - buttonWidth);

                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

                        ImVec4 powerIconColor = (isOn) ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                        ImGui::PushStyleColor(ImGuiCol_Text, powerIconColor);

                        if (ImGui::Button(ICON_FA_POWER_OFF, ImVec2(buttonWidth, buttonWidth))) {
                            if (!isOn) {
                                m_TimersList.at(i).Start();
                                isOn = true;
                                isPlaying = true;
                            } else {
                                m_TimersList.at(i).Reset();
                                isOn = false;
                                isPlaying = false;
                            }
                        }

                        ImGui::PopStyleColor();

                        if (isOn) {
                            ImGui::SameLine(availableWidth - totalButtonWidth);

                            if (!isPlaying) {
                                if (ImGui::Button(ICON_FA_PLAY, ImVec2(buttonWidth, buttonWidth))) {
                                    m_TimersList.at(i).Play();
                                    isPlaying = true;
                                }
                            } else {
                                if (ImGui::Button(ICON_FA_PAUSE, ImVec2(buttonWidth, buttonWidth))) {
                                    m_TimersList.at(i).Pause();
                                    isPlaying = false;
                                }
                            }
                        }

                        ImGui::PopStyleColor(3);
                        ImGui::Separator();
                    }
                    ImGui::End();
                }
            };
        }

        virtual ~ToolsEditorLayer() = default;

        void OnRender() override {
            m_renderFunction();
        }

    private:
        std::function<void()> m_renderFunction;
        bool showWindow = false;
        bool isPlaying = true;
        bool isOn = true;
};
#pragma once

#include <LayerEditor.hpp>
#include <LayerStackEditor.hpp>

#include <CircularBuffer.hpp>
#include <LogsManager.hpp>

class LogsEditorLayer : public LayerEditor, public LogsManager, public std::streambuf {
    public:
        LogsEditorLayer(std::string name) : LayerEditor(name), circularBuffer(50) {
            originalBuffer = std::cout.rdbuf(this);

            m_renderFunction = [this]() {
                ImGui::Begin(ICON_FA_HISTORY "  Console Output");

                std::lock_guard<std::mutex> guard(mutex);
                size_t size = circularBuffer.getSize();
                for (size_t i = 0; i < size; ++i) {
                    std::string line = circularBuffer.dequeue();
                    ImGui::TextUnformatted(line.c_str());
                    circularBuffer.enqueue(line);
                }

                ImGui::End();
            };
        }

        ~LogsEditorLayer() override {
            std::cout.rdbuf(originalBuffer);
        }

        void OnRender() override {
            m_renderFunction();
        }

    protected:
        int overflow(int c) override {
            if (c != EOF) {
                std::lock_guard<std::mutex> guard(mutex);
                char ch = static_cast<char>(c);
                if (ch == '\n') {
                    if (!currentLine.empty()) {
                        // Filter out unwanted text here
                        std::string filteredLine = removeAnsiColorCodes(currentLine);
                        if (!filteredLine.empty()) {
                            circularBuffer.enqueue(filteredLine);
                        }
                        currentLine.clear();
                    }
                } else {
                    currentLine += ch;
                }
            }
            return c;
        }

    private:
        std::string removeAnsiColorCodes(const std::string& text) {
            std::string result;
            bool inColorCode = false;
            for (char ch : text) {
                if (ch == '\033') { 
                    inColorCode = true;
                } else if (inColorCode && ch == 'm') {
                    inColorCode = false;
                } else if (!inColorCode) {
                    result += ch;
                }
            }

            return result;
        }

        std::function<void()> m_renderFunction;
        std::streambuf* originalBuffer = nullptr;
        CircularBuffer<std::string> circularBuffer;
        std::string currentLine;
        std::mutex mutex;
};
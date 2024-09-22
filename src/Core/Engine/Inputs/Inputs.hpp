#pragma once

#include <HeadersBase.hpp>

#include <EventHandler.hpp>
#include <KeyEvents.hpp>
#include <MouseEvents.hpp>

#include <LogsManager.hpp>

class Inputs : public LogsManager {
    public:
        Inputs(GLFWwindow* window);
        ~Inputs() = default;

        void OnInitInputEvents();
        bool IsKeyPressed(int keyCode) const;

        // Keys
        void OnKeyPressed(const KeyPressedEvent& e);
        void OnKeyReleased(const KeyReleasedEvent& e);
        void TriggerKeyPressedEvent(int keyCode, int repeatCount);
        void TriggerKeyReleasedEvent(int keyCode);
        void setKeyCallback();

        // Mouse
        void OnMouseMoved(const MouseMovedEvent& e);
        void TriggerMouseMovedEvent(float x, float y);
        void setMouseMovedCallback();

        void OnMouseScrolled(const MouseScrolledEvent& e);
        void TriggerMouseScrolledEvent(float x, float y);
        void setMouseScrolledCallback();

        void OnMouseButtonPressed(const MouseButtonPressedEvent& e);
        void TriggerMouseButtonPressedEvent(int button);
        void OnMouseButtonReleased(const MouseButtonReleasedEvent& e);
        void TriggerMouseButtonReleasedEvent(int button);
        void setMouseButtonCallback();

    private:
        GLFWwindow* m_window;
        
        // Key
        EventHandler<KeyPressedEvent> m_keyPressed = [this](const KeyPressedEvent& e) { OnKeyPressed(e); };
        EventHandler<KeyReleasedEvent> m_keyReleased = [this](const KeyReleasedEvent& e) { OnKeyReleased(e); };

        // Mouse
        EventHandler<MouseMovedEvent> m_mouseMoved = [this](const MouseMovedEvent& e) { OnMouseMoved(e); };
        EventHandler<MouseScrolledEvent> m_mouseScrolled = [this](const MouseScrolledEvent& e) { OnMouseScrolled(e); };
        EventHandler<MouseButtonPressedEvent> m_mouseButtonPressed = [this](const MouseButtonPressedEvent& e) { OnMouseButtonPressed(e); };
        EventHandler<MouseButtonReleasedEvent> m_mouseButtonReleased = [this](const MouseButtonReleasedEvent& e) { OnMouseButtonReleased(e); };

        // Conteneur pour les touches pressées
        std::unordered_set<int> m_pressedKeys;

        // Faire pareil pour les touches released / souris pressed et released
};
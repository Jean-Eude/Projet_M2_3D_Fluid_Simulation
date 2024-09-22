#pragma once

#include <Event.hpp>
#include <MouseCodes.hpp>

class MouseMovedEvent : public Event {
    public:
        EVENT_TYPE("MouseMovedEvent")

        MouseMovedEvent(float x, float y) : mouseX(x), mouseY(y) {
        }

        std::string ToString() const override
        {
            return "MouseMovedEvent: {" + std::to_string(mouseX) + "}, {" + std::to_string(mouseY) + "}";
        }

        float mouseX { 0.0f };
        float mouseY { 0.0f };
};

class MouseScrolledEvent : public Event {
    public:
        EVENT_TYPE("MouseScrolledEvent")

        MouseScrolledEvent(float xOffset_, float yOffset_) : xOffset(xOffset_), yOffset(yOffset_) {
        }

        
        std::string ToString() const override
        {
            return "MouseScrolledEvent: {" + std::to_string(xOffset) + "}, {" + std::to_string(yOffset) + "}";
        }

        float xOffset { 0.0f };
        float yOffset { 0.0f };
};

class MouseButtonPressedEvent : public Event {
    public:
        EVENT_TYPE("MouseButtonPressedEvent")

        MouseButtonPressedEvent(int button) : button(button) {
        }

    
        std::string ToString() const override
        {
            return "MouseButtonPressedEvent: {" + std::to_string(button) + "}";
        }

        uint16_t button { 0 };
};

class MouseButtonReleasedEvent : public Event {
    public:
        EVENT_TYPE("MouseButtonReleasedEvent")

        MouseButtonReleasedEvent(int button) : button(button) {
        }

        
        std::string ToString() const override
        {
            return "MouseButtonReleasedEvent: {" + std::to_string(button) + "}";
        }

        uint16_t button { 0 };
};
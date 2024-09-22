#pragma once

#include <KeyCodes.hpp>
#include <Event.hpp>

class KeyPressedEvent : public Event {
    public:
        EVENT_TYPE("KeyPressedEvent")

        KeyPressedEvent(int keyCode, int repeatCount)
            : key(keyCode)
            , repeatCount(repeatCount)
        {
        }

        
        std::string ToString() const override
        {
            return "KeyPressedEvent: " + std::to_string(key) + " (" + std::to_string(repeatCount) + " repeats)";
        }
        
        uint16_t key { 0 };
        int repeatCount { 0 };
};

class KeyReleasedEvent : public Event {
    public:
        EVENT_TYPE("KeyReleasedEvent")

        KeyReleasedEvent(int keyCode)
            : key(keyCode)
        {
        }

        std::string ToString() const override
        {
            return "KeyReleasedEvent: " + std::to_string(key);
        }

        uint16_t key { 0 };
};

class KeyTypedEvent : public Event {
    public:
        EVENT_TYPE("KeyTypedEvent")

        KeyTypedEvent(int keyCode)
            : key(keyCode)
        {
        }

        std::string ToString() const override
        {
            return "KeyTypedEvent: " + std::to_string(key);
        }

        uint16_t key { 0 };
};
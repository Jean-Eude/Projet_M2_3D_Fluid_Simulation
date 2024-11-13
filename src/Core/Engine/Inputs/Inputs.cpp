#include <Inputs.hpp>
#include <EventManager.hpp>

Inputs::Inputs(GLFWwindow* window) : m_window(window) {
    OnInitInputEvents();
    setKeyCallback();
    setMouseMovedCallback();
    setMouseScrolledCallback();
    setMouseButtonCallback();
}

void Inputs::OnInitInputEvents() {
    Subscribe<KeyPressedEvent>(m_keyPressed, 0);
    Subscribe<KeyReleasedEvent>(m_keyReleased, 0);
    Subscribe<MouseMovedEvent>(m_mouseMoved, 0);
    Subscribe<MouseScrolledEvent>(m_mouseScrolled, 0);
    Subscribe<MouseButtonPressedEvent>(m_mouseButtonPressed, 0);
    Subscribe<MouseButtonReleasedEvent>(m_mouseButtonReleased, 0);
}

bool Inputs::IsKeyPressed(int keyCode) const {
    return m_pressedKeys.find(keyCode) != m_pressedKeys.end();
}

// Key
void Inputs::OnKeyPressed(const KeyPressedEvent& e) {
    m_pressedKeys.insert(e.key);
    for (auto& listener : keyPressedListeners) {
        listener(e);
    }

    // Pas besoin de timing pour les logs des inputs
    //LogsManager::enqueueConsoleLogsInit(e.ToString(), LogLevel::DEBUG);
}

void Inputs::OnKeyReleased(const KeyReleasedEvent& e) {
    m_pressedKeys.erase(e.key);
    for (auto& listener : keyReleasedListeners) {
        listener(e);
    }

    //LogsManager::enqueueConsoleLogsInit(e.ToString(), LogLevel::DEBUG);
}

void Inputs::TriggerKeyPressedEvent(int keyCode, int repeatCount) {
    KeyPressedEvent keyEvent(keyCode, repeatCount);
    gEventManager.TriggerEvent(keyEvent, 0);
}

void Inputs::TriggerKeyReleasedEvent(int keyCode) {
    KeyReleasedEvent keyEvent(keyCode);
    gEventManager.TriggerEvent(keyEvent, 0);
}

void Inputs::setKeyCallback() {
    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        Inputs* self = static_cast<Inputs*>(glfwGetWindowUserPointer(window));
        if (self) {
            if (action == GLFW_PRESS) {
                self->TriggerKeyPressedEvent(key, 0);
            } else if (action == GLFW_RELEASE) {
                self->TriggerKeyReleasedEvent(key);
            }
        }
    });

    glfwSetWindowUserPointer(m_window, this);
}

void Inputs::setKeyPressedListener(const Inputs::EventListener<KeyPressedEvent>& listener) {
    keyPressedListeners.push_back(listener);
}

void Inputs::setKeyReleasedListener(const Inputs::EventListener<KeyReleasedEvent>& listener) {
    keyReleasedListeners.push_back(listener);
}

// Mouse
void Inputs::OnMouseMoved(const MouseMovedEvent& e) {
    m_mousePosition = glm::vec2(e.mouseX, e.mouseY);
    for (auto& listener : mouseMoveListeners) {
        listener(e);
    }
}

void Inputs::TriggerMouseMovedEvent(float x, float y) {
    MouseMovedEvent mouseEvent(x, y);
    gEventManager.TriggerEvent(mouseEvent, 0);   
}

void Inputs::setMouseMovedCallback() {
    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
        Inputs* self = static_cast<Inputs*>(glfwGetWindowUserPointer(window));
        if (self) {
            self->TriggerMouseMovedEvent(static_cast<float>(xpos), static_cast<float>(ypos));
        }
    });

    glfwSetWindowUserPointer(m_window, this);
}

void Inputs::setMouseMovedListener(const Inputs::EventListener<MouseMovedEvent>& listener) {
    mouseMoveListeners.push_back(listener);
}

void Inputs::OnMouseScrolled(const MouseScrolledEvent& e) {
    for (auto& listener : mouseScrolledListeners) {
        listener(e);
    }
}

void Inputs::TriggerMouseScrolledEvent(float x, float y) {
    MouseScrolledEvent mouseEvent(x, y);
    gEventManager.TriggerEvent(mouseEvent, 0);   
}

void Inputs::setMouseScrolledCallback() {
    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
        Inputs* self = static_cast<Inputs*>(glfwGetWindowUserPointer(window));
        if (self) {
            self->TriggerMouseScrolledEvent(static_cast<float>(xoffset), static_cast<float>(yoffset));
        }
    });

    glfwSetWindowUserPointer(m_window, this);
}

void Inputs::setMouseScrolledListener(const Inputs::EventListener<MouseScrolledEvent>& listener) {
    mouseScrolledListeners.push_back(listener);
}

void Inputs::OnMouseButtonPressed(const MouseButtonPressedEvent& e) {
    for (auto& listener : mouseButtonPressedListeners) {
        listener(e);
    }
}

void Inputs::TriggerMouseButtonPressedEvent(int button) {
    MouseButtonPressedEvent mouseEvent(button);
    gEventManager.TriggerEvent(mouseEvent, 0); 
}


void Inputs::OnMouseButtonReleased(const MouseButtonReleasedEvent& e) {
    for (auto& listener : mouseButtonReleasedListeners) {
        listener(e);
    }
}

void Inputs::TriggerMouseButtonReleasedEvent(int button) {
    MouseButtonReleasedEvent mouseEvent(button);
    gEventManager.TriggerEvent(mouseEvent, 0); 
}

void Inputs::setMouseButtonCallback() {
    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
        Inputs* self = static_cast<Inputs*>(glfwGetWindowUserPointer(window));
        if (self) {
            if(action == GLFW_PRESS) {
                self->TriggerMouseButtonPressedEvent(button);
            } else if(action == GLFW_RELEASE) {
                self->TriggerMouseButtonReleasedEvent(button);
            }
        }
    });

    glfwSetWindowUserPointer(m_window, this);
}

void Inputs::setMouseButtonPressedListener(const Inputs::EventListener<MouseButtonPressedEvent>& listener) {
    mouseButtonPressedListeners.push_back(listener);
}

void Inputs::setMouseButtonReleasedListener(const Inputs::EventListener<MouseButtonReleasedEvent>& listener) {
    mouseButtonReleasedListeners.push_back(listener);
}

glm::vec2 Inputs::getMousePosition() const {
    return m_mousePosition;
}
#pragma once

#include <Event.hpp>
#include <LayerEditor.hpp>
#include <LayerEngine.hpp>

// Editor
class LayerEditorPushed : public Event {
    public:
        EVENT_TYPE("LayerEditorPushed")

        LayerEditorPushed(LayerEditor* layer) : layer(layer) {
        }

        
        std::string ToString() const override
        {
            return "LayerEditorPushed: " + layer->getName();
        }
        
        LayerEditor* layer;
};

class LayerEditorPoped : public Event {
    public:
        EVENT_TYPE("LayerEditorPoped")

        LayerEditorPoped(LayerEditor* layer) : layer(layer) {
        }

        
        std::string ToString() const override
        {
            return "LayerEditorPoped: " + layer->getName();
        }
        
        LayerEditor* layer;
};

class OverlayEditorPushed : public Event {
    public:
        EVENT_TYPE("OverlayEditorPushed")

        OverlayEditorPushed(LayerEditor* layer) : layer(layer) {
        }

        
        std::string ToString() const override
        {
            return "OverlayEditorPushed: " + layer->getName();
        }
        
        LayerEditor* layer;
};

class OverlayEditorPoped : public Event {
    public:
        EVENT_TYPE("OverlayEditorPoped")

        OverlayEditorPoped(LayerEditor* layer) : layer(layer) {
        }

        
        std::string ToString() const override
        {
            return "OverlayEditorPoped: " + layer->getName();
        }
        
        LayerEditor* layer;
};

// Engine
class LayerEnginePushed : public Event {
    public:
        EVENT_TYPE("LayerEnginePushed")

        LayerEnginePushed(LayerEngine* layer) : layer(layer) {
        }

        
        std::string ToString() const override
        {
            return "LayerEnginePushed: " + layer->getName();
        }
        
        LayerEngine* layer;
};

class LayerEnginePoped : public Event {
    public:
        EVENT_TYPE("LayerEditorPoped")

        LayerEnginePoped(LayerEngine* layer) : layer(layer) {
        }

        
        std::string ToString() const override
        {
            return "LayerEnginePoped: " + layer->getName();
        }
        
        LayerEngine* layer;
};

class OverlayEnginePushed : public Event {
    public:
        EVENT_TYPE("OverlayEditorPushed")

        OverlayEnginePushed(LayerEngine* layer) : layer(layer) {
        }

        
        std::string ToString() const override
        {
            return "OverlayEnginePushed: " + layer->getName();
        }
        
        LayerEngine* layer;
};

class OverlayEnginePoped : public Event {
    public:
        EVENT_TYPE("OverlayEditorPoped")

        OverlayEnginePoped(LayerEngine* layer) : layer(layer) {
        }

        
        std::string ToString() const override
        {
            return "OverlayEnginePoped: " + layer->getName();
        }
        
        LayerEngine* layer;
};

#pragma once

#include <HeadersBase.hpp>

class LayerEditor {
    public:
        LayerEditor(const std::string& name = "LayerEditor") : m_DebugName(name) {}
        virtual ~LayerEditor() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}

        virtual void OnInit() {}
        virtual void OnRender() {}

        std::string getName() {return m_DebugName;}

    protected:
        std::string m_DebugName;
};
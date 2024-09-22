#pragma once

#include <HeadersBase.hpp>

class LayerEngine {
    public:
        LayerEngine(const std::string& name = "LayerEngine") : m_DebugName(name) {}
        virtual ~LayerEngine() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnRender() {}

        std::string getName() {return m_DebugName;}

    protected:
        std::string m_DebugName;
};
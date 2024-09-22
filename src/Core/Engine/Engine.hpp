#pragma once

#include <EngineManager.hpp>

class Engine {
    public:
        Engine();
        ~Engine() = default;

        void OnInit();
        void OnUpdate();
        void OnDestroy();

    private:
        EngineManager engine_mng;
};
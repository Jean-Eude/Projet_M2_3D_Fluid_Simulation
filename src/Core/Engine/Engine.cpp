#include <Engine.hpp>

Engine::Engine() {
    
}

void Engine::OnInit() {
    engine_mng.OnInitWindowEngine();   
}

void Engine::OnUpdate() {
    while (!glfwWindowShouldClose(engine_mng.getWindow()) && !engine_mng.getDone()) {
        engine_mng.OnUpdateWindowEngine();
    }
}

void Engine::OnDestroy() {
    engine_mng.OnDestroyWindowEngine();
}
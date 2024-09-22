#include <Engine.hpp>
#include <CircularBuffer.hpp>

int main() {
    Engine eng;

    eng.OnInit();
    eng.OnUpdate();
    eng.OnDestroy();
}
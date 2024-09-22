#include <Timer.hpp>

Timer::Timer(std::string name) : m_startTime(std::chrono::high_resolution_clock::now()), m_lastTime(m_startTime), m_deltaTime(0.0), m_totalTimeForFPS(0.0), m_Running(false), name(name), m_MS_Per_Update(0.016), m_MS_BIS_Per_Update(0.017), m_FPS(0.), m_MS(0.), m_nbFrames(0), m_Acc(0.0) {

}

// Démarage du timer
void Timer::Start() {
    if (!m_Running) {
        m_Running = true;
        m_lastTime = std::chrono::high_resolution_clock::now();
    }
}

// Redémarrage du timer après l'avoir stoppé
void Timer::Play() {
    m_startTime = std::chrono::high_resolution_clock::now();
    m_lastTime = m_startTime;
    m_deltaTime = 0.0;
    m_Running = true;    
}

// Mise en pause du timer
void Timer::Pause() {
    if (m_Running) {
        auto now = std::chrono::high_resolution_clock::now();
        m_totalTime += std::chrono::duration<double>(now - m_lastTime).count();
        m_lastTime = now;
        m_Running = false;
    }    
}

// Remise à zéro du timer
void Timer::Reset() {
    m_startTime = std::chrono::high_resolution_clock::now();
    m_lastTime = m_startTime;
    m_totalTime = 0.0;
    m_deltaTime = 0.0;
    m_Running = false;
}

void Timer::Update() {
    if (m_Running) {
        auto now = std::chrono::high_resolution_clock::now();
        m_deltaTime = std::chrono::duration<double>(now - m_lastTime).count();
        m_totalTimeForFPS += m_deltaTime;
        m_totalTime += m_deltaTime;
        m_lastTime = now;

        m_Acc+=m_deltaTime;
        m_nbFrames+=1;

        if (m_totalTimeForFPS >= 1.0) { 
            m_FPS = m_nbFrames / m_totalTimeForFPS;
            m_MS = 1000.0 / m_FPS;  // MS par frame = 1000 / FPS
            m_nbFrames = 0;
            m_totalTimeForFPS = 0.0;
        }
    }
}

// Getters
double Timer::getDeltaTime() {
    return m_deltaTime;
}

double Timer::getTotalTimeinMilliSeconds() {
    return m_totalTime * 1000.;
}

double Timer::getTotalTimeinSeconds() {
    return m_totalTime;
}

double Timer::getTotalTimeinMinutes() {
    return m_totalTime / 60;
}

double Timer::getTotalTimeinHours() {
    return m_totalTime / 3600;
}

double Timer::getAcc() {
    return m_Acc;
}

double Timer::getFPS() {
    return m_FPS;
}

double Timer::getMS() {
    return m_MS;
}

double Timer::getMSPerUpdate() {
    return m_MS_Per_Update;
}

std::string Timer::getName() {
    return name;
}

// Metrics
void Timer::UpdateDeltaTime() {
    m_Acc -= m_MS_BIS_Per_Update;

    if(getAcc() < (m_MS_Per_Update - 1.) - m_MS_BIS_Per_Update) {
        m_Acc = 0.;    
    }  
}
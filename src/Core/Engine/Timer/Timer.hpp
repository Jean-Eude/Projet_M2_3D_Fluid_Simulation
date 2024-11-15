#pragma once

// Classe pour gérer le temps

#include <HeadersBase.hpp>

class Timer {
    public:
        Timer(std::string name);
        ~Timer() = default;

        // Fonctions qui permettent de gérer le timer
        void Start();
        void Play();
        void Pause();
        void Reset();
        void Update();

        // Getters
        double getDeltaTime();
        double getTotalTimeinMilliSeconds();
        double getTotalTimeinSeconds();
        double getTotalTimeinMinutes();
        double getTotalTimeinHours();
        double getAcc();
        double getFPS();
        double getMS();
        double getMSPerUpdate();
        std::string getName();

        void setAcc(double newAcc);

        // Metrics
        void UpdateDeltaTime();

    private:
        // Variables
        double m_deltaTime;
        double m_totalTime, m_totalTimeForFPS;

        std::chrono::high_resolution_clock::time_point m_startTime;
        std::chrono::high_resolution_clock::time_point m_lastTime = m_startTime;
        std::chrono::high_resolution_clock::time_point m_currentTime;

        bool m_Running;

        std::string name;

        // Varibales (FPS, MS, Acc, etc...)
        double m_Acc;
        double m_MS_Per_Update, m_MS_BIS_Per_Update;
        double m_FPS, m_MS;
        int m_nbFrames;
};


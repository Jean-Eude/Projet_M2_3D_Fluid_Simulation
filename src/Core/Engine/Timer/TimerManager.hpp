#pragma once

#include <Timer.hpp>

class TimerManager {
    public:
        virtual ~TimerManager() = default;
        void addTimer(std::string name);

        static std::vector<Timer> m_TimersList;
};
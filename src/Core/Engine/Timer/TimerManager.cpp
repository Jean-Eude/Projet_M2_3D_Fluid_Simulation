#include <TimerManager.hpp>

std::vector<Timer> TimerManager::m_TimersList;

void TimerManager::addTimer(std::string name) {
    Timer timer(name);
    this->m_TimersList.push_back(timer);
}
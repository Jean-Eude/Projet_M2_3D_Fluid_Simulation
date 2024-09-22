#pragma once

#include <EventHandler.hpp>

using EventType = std::uint32_t;
using EventId = std::uint64_t;

class EventManager {
public:
    EventManager() = default;
    EventManager(const EventManager&) = delete;
    const EventManager& operator=(const EventManager&) = delete;

    void Shutdown();

    void Subscribe(EventType eventType, std::unique_ptr<IEventHandlerWrapper>&& handler, EventId eventId);
    void Unsubscribe(EventType eventType, const std::string& handlerName, EventId eventId);
    void TriggerEvent(const Event& event_, EventId eventId);
    void QueueEvent(std::unique_ptr<Event>&& event_, EventId eventId);
    void DispatchEvents();

private:
    std::vector<std::pair<std::unique_ptr<Event>, EventId>> m_eventsQueue;
    std::unordered_map<EventType, std::vector<std::unique_ptr<IEventHandlerWrapper>>> m_subscribers;
    std::unordered_map<EventType, std::unordered_map<EventId, std::vector<std::unique_ptr<IEventHandlerWrapper>>>> m_subscribersByEventId;
};

extern EventManager gEventManager;

template<typename EventType>
inline void Subscribe(const EventHandler<EventType>& callback, EventId eventId = 0, const bool unsubscribeOnSuccess = false)
{
    std::unique_ptr<IEventHandlerWrapper> handler = std::make_unique<EventHandlerWrapper<EventType>>(callback, unsubscribeOnSuccess);
    gEventManager.Subscribe(EventType::GetStaticEventType(), std::move(handler), eventId);
}

template<typename EventType>
inline void Unsubscribe(const EventHandler<EventType>& callback, EventId eventId = 0)
{
    const std::string handlerName = callback.target_type().name();
    gEventManager.Unsubscribe(EventType::GetStaticEventType(), handlerName, eventId);
}

inline void TriggerEvent(const Event& triggeredEvent, EventId eventId = 0)
{
    gEventManager.TriggerEvent(triggeredEvent, eventId);
}

inline void QueueEvent(std::unique_ptr<Event>&& queuedEvent, EventId eventId = 0)
{
    gEventManager.QueueEvent(std::forward<std::unique_ptr<Event>>(queuedEvent), eventId);
}

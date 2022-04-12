#pragma once

#include "MessageDispatcher.hpp"
#include "MessageSubscriber.hpp"

namespace gjt
{
template<typename MessageType, typename Message>
void MessageDispatcher<MessageType, Message>::subscribe(
    const MessageType type,
    MessageSubscriber<MessageType, Message> *subscriber)
{
    auto itr = subscribers.find(type);
    if (itr == subscribers.end())
        subscribers[type] = {subscriber};
    else
        subscribers[type].push_back(subscriber);
}

template <typename MessageType, typename Message>
void MessageDispatcher<MessageType, Message>::broadcast(
    const Message &message,
    MessageSubscriber<MessageType, Message> *subscriber)
{
    auto itr = subscribers.find(message.type);
    if (itr != subscribers.end())
    {
        for (auto s : itr->second)
        {
            if (subscriber == NULL || subscriber != s)
                s->receiveMessage(message);
        }
    }
}

template <typename MessageType, typename Message>
void MessageDispatcher<MessageType, Message>::clear()
{
    subscribers.clear();
}
} // namespace gjt

#pragma once

#include "MessageSubscriber.hpp"
#include "MessageDispatcher.hpp"
#include <memory>

namespace gjt
{
template<typename MessageType, typename Message>
MessageSubscriber<MessageType, Message>::MessageSubscriber(
    MessageDispatcher<MessageType, Message> *dispatcher)
    : inbox(), messages(), inboxCount(0), messageCount(0),
      dispatcher(dispatcher)
{
}

template <typename MessageType, typename Message>
MessageSubscriber<MessageType, Message>::~MessageSubscriber()
{
}

template <typename MessageType, typename Message>
void MessageSubscriber<MessageType, Message>::process(float dt)
{
    if (inboxCount == 0)
        return;
    memcpy(messages, inbox, sizeof(Message) * inboxCount);
    messageCount = inboxCount;
    processMessages(messages, messageCount, dt);
    inboxCount = 0;
    messageCount = 0;
}

template <typename MessageType, typename Message>
void MessageSubscriber<MessageType, Message>::receiveMessage(
    const Message &message)
{
    assert(inboxCount < 64);
    inbox[inboxCount++] = message;
}
} // namespace gjt

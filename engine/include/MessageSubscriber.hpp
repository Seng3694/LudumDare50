#pragma once

#include <cassert>
#include <memory>

namespace gjt
{
template <typename MessageType, typename Message> 
class MessageDispatcher;

template <typename MessageType, typename Message> 
class MessageSubscriber
{
    template <typename MessageType, typename Message> 
    friend class MessageDispatcher;

  public:
    MessageSubscriber(MessageDispatcher<MessageType, Message> *dispatcher);
    virtual ~MessageSubscriber();
    void process(float dt);

  protected:
    MessageDispatcher<MessageType, Message> *dispatcher;

    virtual void processMessages(
        const Message *messages, const uint32_t messageCount, float dt) = 0;

  private:
    void receiveMessage(const Message &message);

    Message inbox[64];
    Message messages[64];
    uint32_t inboxCount;
    uint32_t messageCount;
};
} // namespace gjt

#include "MessageSubscriber.inl"

#pragma once

#include "MessageSubscriber.hpp"

#include <unordered_map>
#include <vector>

namespace gjt
{
template <typename MessageType, typename Message> 
class MessageSubscriber;

template <typename MessageType, typename Message> 
class MessageDispatcher
{
  public:
    void subscribe(const MessageType type, MessageSubscriber<MessageType, Message> *subscriber);
    void broadcast(const Message &message, MessageSubscriber<MessageType, Message> *subscriber = NULL);
    void clear();

  private:
    std::unordered_map<
        MessageType, std::vector<MessageSubscriber<MessageType, Message>*>>
        subscribers;
};

}

#include "MessageDispatcher.inl"

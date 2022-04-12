#pragma once

#include <SFML/Graphics.hpp>

enum class MessageType
{
    None,
	EventMessageType,
	PlayerHitMessageType,
	GrassMownMessageType,
	Count
};

struct Message
{
    MessageType type;

	struct PlayerHitMessage
    {
        int32_t maxHp;
        int32_t hp;
	};

	struct GrassMownMessage
    {
        sf::Vector2f position;
        int32_t grassMownTotal;
        int32_t grassMownMax;
	};

	union {
        sf::Event event;
        PlayerHitMessage playerHit;
        GrassMownMessage grassMown;
	};

    Message() : type(MessageType::None){}
    Message(const sf::Event &e) : type(MessageType::EventMessageType), event(e)
    {
    }
    Message(const PlayerHitMessage &playerHit)
        : type(MessageType::PlayerHitMessageType), playerHit(playerHit)
    {
    }
    Message(const GrassMownMessage &grassMown)
        : type(MessageType::GrassMownMessageType), grassMown(grassMown)
    {
    }
};

#include "MessageDispatcher.hpp"
#include "MessageSubscriber.hpp"

typedef gjt::MessageDispatcher<MessageType, Message> GameMessageDispatcher;
typedef gjt::MessageSubscriber<MessageType, Message> GameMessageSubscriber;

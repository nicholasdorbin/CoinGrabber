#pragma once
#include "Engine/Net/Session/NetSession.hpp"
#include "Game/Net/NetPlayerData.hpp"

//class NetConnection;

enum eGamePackets : uint8_t
{
	eMessageID_NET_GAME_UPDATE_HOST_TO_CLIENT = eMessageID_MAX_CORE_MESSAGE,
	eMessageID_NET_GAME_UPDATE_CLIENT_TO_HOST,
	eMessageID_NET_GAME_TEST_RELIABLE,
	eMessageID_NET_GAME_TEST_IN_ORDER,
	eMessageID_NET_GAME_REQUEST_PLAYER,
	eMessageID_NET_GAME_PLAYER_JOINED,
	eMessageID_NET_GAME_PLAYER_DENY,
	eMessageID_NET_GAME_PLAYER_LEAVE,


	//Net Objects
	eMessageID_NET_GAME_OBJECT_CREATE,
	eMessageID_NET_GAME_OBJECT_DESTROY,
	eMessageID_NET_GAME_OBJECT_UPDATE,

	//NetTime
	eMessageID_NET_GAME_NET_TIME_UPDATE
};

#define GAME_NET_TIME_UPDATE_TICKRATE 1.f
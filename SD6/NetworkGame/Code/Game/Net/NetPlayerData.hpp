#pragma once
#include <stdint.h>

class NetMessage;

struct NetPlayerData
{
	// Must haves
	uint8_t m_playerIndex;            // player index (or guid) assigned by the game to you
	uint8_t m_ownerConnectionIndex;  // index of who originated this player.
	uint32_t m_creationNuonce;        // only used to tie this back to my creation request

									 // Player contains game specific properties - for example.
	//char name[32];
	//color_fl color;

	//uint8_t team_index;
	//bool is_ready;       // They have readied up and can move on
	//bool is_in_menu;     // if I wanted to put an icon over their head if they were in menu

	NetPlayerData();

	void WritePlayerDataToMessage(NetMessage* msg);
	void ReadPlayerDataFromMessage(NetMessage* msg);
};
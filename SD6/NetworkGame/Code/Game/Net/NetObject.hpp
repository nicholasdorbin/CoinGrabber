#pragma once
#include <stdint.h>

class NetObjectType;

class NetObject
{
public:
	uint16_t net_id;

	uint8_t owning_player_index;
	uint8_t owning_connection_index;
	uint32_t type_id;

	NetObjectType *type;
	void *game_object;
};

#pragma once
#include "Engine/Math/Vector2.hpp"
#include <stdint.h>

class NetGameObjectParent
{
public:
	NetGameObjectParent();
	uint16_t m_netObjectId; //This is unique for each object so clients can update it accordingly
	uint8_t m_ownerNetIdx;
	uint32_t m_creationNuonce;

};
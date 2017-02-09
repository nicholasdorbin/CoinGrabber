#pragma once
//All the Game Specific includes
#include "Game/Net/NetObjectTypeCoin.hpp"

#include <map>
enum eNetObjectTypes : uint32_t
{
	eNetObjectTypes_COIN = 0
};

class NetTypesSystem
{
public:
	//map of NetObjectTypes
	NetTypesSystem();
	~NetTypesSystem();
	std::map<eNetObjectTypes, NetObjectType*> m_NetObjectTypeRegistry;

	void RegisterAllNetObjectTypes();
	NetObjectType* FindNetObjectType(eNetObjectTypes typeEnum);
private:
	void RegisterNetObjectType(eNetObjectTypes typeEnum, NetObjectType* typePointer);
};
#include "Game/Net/NetObjectTypesSystem.hpp"

NetTypesSystem::NetTypesSystem()
{

}

NetTypesSystem::~NetTypesSystem()
{
	auto iter = m_NetObjectTypeRegistry.begin();
	while (iter != m_NetObjectTypeRegistry.end())
	{
		delete iter->second;
		iter = m_NetObjectTypeRegistry.erase(iter);
	}
}

void NetTypesSystem::RegisterAllNetObjectTypes()
{
	RegisterNetObjectType(eNetObjectTypes_COIN, new NetObjectTypeCoin());
}

NetObjectType* NetTypesSystem::FindNetObjectType(eNetObjectTypes typeEnum)
{
	NetObjectType* found = nullptr;

	found = m_NetObjectTypeRegistry.find(typeEnum)->second;

	return found;
}

void NetTypesSystem::RegisterNetObjectType(eNetObjectTypes typeEnum, NetObjectType* typePointer)
{
	m_NetObjectTypeRegistry.insert(std::pair<eNetObjectTypes, NetObjectType*>(typeEnum, typePointer));
}

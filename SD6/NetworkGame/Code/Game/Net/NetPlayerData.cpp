#include "Game/Net/NetPlayerData.hpp"
#include "Engine/Net/Session/NetMessage.hpp"

NetPlayerData::NetPlayerData()
	: m_playerIndex(0xff)
	, m_ownerConnectionIndex(0xff)
	, m_creationNuonce(0xffff)
{
}

void NetPlayerData::WritePlayerDataToMessage(NetMessage* msg)
{
	//WriteToMessage
	msg->WriteToMessage<uint8_t>(&m_playerIndex);
	msg->WriteToMessage<uint8_t>(&m_ownerConnectionIndex);
	msg->WriteToMessage<uint32_t>(&m_creationNuonce);
}

void NetPlayerData::ReadPlayerDataFromMessage(NetMessage* msg)
{
	msg->ResetOffset();
	msg->ReadFromMessage<uint8_t>(&m_playerIndex);
	msg->ReadFromMessage<uint8_t>(&m_ownerConnectionIndex);
	msg->ReadFromMessage<uint32_t>(&m_creationNuonce);
}

#include "Game/Net/NetObjectTypeCoin.hpp"
#include "Game/Coin.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Net/Session/NetMessage.hpp"
#include "Game/Net/NetObject.hpp"

void* NetObjectTypeCoin::OnCreate(NetMessage *create_msg)
{
	Coin* coin = new Coin();
	Vector2 newPos = Vector2::ZERO;
	create_msg->ReadFromMessage<Vector2>(&newPos);
	coin->m_pos = newPos;
	coin->m_initialPos = newPos;
	return coin;
}

void NetObjectTypeCoin::SendCreate(NetObject *object, NetMessage* msg)
{
	Coin *coin = (Coin*)object->game_object;

	msg->WriteToMessage<Vector2>(&coin->m_pos);
	msg->WriteToMessage<bool>(&coin->m_isDead);

}

void NetObjectTypeCoin::SendClientUpdate(NetObject *object, NetMessage* msg)
{
	
}

void NetObjectTypeCoin::ProcessClientUpdate(NetObject *object, NetMessage*  update)
{
	Coin *coin = (Coin*)object->game_object;
	Vector2 newPos = Vector2::ZERO;
	bool isDead = coin->m_isDead;
	update->ReadFromMessage<Vector2>(&newPos);
	update->ReadFromMessage<bool>(&isDead);
	coin->m_pos = newPos;
	coin->m_isDead = isDead;
}

void NetObjectTypeCoin::SendHostUpdate(NetObject *object, NetMessage* msg) const
{
	Coin *coin = (Coin*)object->game_object;
	Vector2* posPointer = &coin->m_pos;
	bool isDead = coin->m_isDead;
	msg->WriteToMessage<Vector2>(posPointer);
	msg->WriteToMessage<bool>(&isDead);
}

void NetObjectTypeCoin::ProcessHostUpdate(NetObject *object, NetMessage*  update) const
{


}

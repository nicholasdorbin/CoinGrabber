#pragma once
#include "Game/Net/NetObjectType.hpp"

class NetObjectTypeCoin : public NetObjectType
{
public:

	virtual void* OnCreate(NetMessage* create_msg);
	virtual void SendCreate(NetObject *object, NetMessage* msg);

	// Sent by clients for objects they own and want
	// to influence
	virtual void SendClientUpdate(NetObject *object, NetMessage* msg);
	virtual void ProcessClientUpdate(NetObject *object, NetMessage*  update);

	// Sent by host for all objects - definitive state update
	virtual void SendHostUpdate(NetObject *object, NetMessage* msg) const;
	virtual void ProcessHostUpdate(NetObject *object, NetMessage* update) const;
};
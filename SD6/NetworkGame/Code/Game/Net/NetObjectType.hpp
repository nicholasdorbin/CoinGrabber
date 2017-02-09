#pragma once
class NetObject;
class NetMessage;

class NetObjectType
{
public:
	virtual void* OnCreate(NetMessage* create_msg) = 0;
	virtual void SendCreate(NetObject *object, NetMessage* msg) = 0;

	// Sent by clients for objects they own and want
	// to influence
	virtual void SendClientUpdate(NetObject *object, NetMessage* msg) = 0;
	virtual void ProcessClientUpdate(NetObject *object, NetMessage* update) = 0;

	// Sent by host for all objects - definitive state update
	virtual void SendHostUpdate(NetObject *object, NetMessage* msg) const = 0;
	virtual void ProcessHostUpdate(NetObject *object, NetMessage* update) const = 0;
};

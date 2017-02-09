#pragma once
#include "Game/Player.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/IntVector2.hpp"
#include <map>
#include <deque>
#include <vector>

class Sprite;
class AnimatedSprite;
class Entity;
class Map;
struct MapRow;
class NetConnection;
class NetMessage;
struct NetSender;
class NetObject;
class NetPlayerData;
class Coin;

const IntVector2 snapBackPos = IntVector2(800, 450);
const float mouseSensitivity = 0.025f;
const float DIST_TO_RAYCAST = 8.f;

const float DESIRED_TIME_THRESHOLD = 0.1f;





class World
{
public:
	World(bool isHostWorld = false);
	~World();
	Player* m_myPlayer;
	std::vector<Player*> m_players;
	Camera3D m_camera;
	float m_moveMod;
	float m_worldTimer;
	float m_mapTick;
	float m_waveSpriteTick;
	Vector3 m_startTrace;
	Vector3 m_endTrace;
	std::vector<Entity*> m_entities;
	std::vector<Entity*> m_entitiesBackGround;
	std::vector<Entity*> m_entitiesToAdd;
	std::vector<Entity*> m_entitiesBackGroundToAdd;
	std::vector<Map*> m_mapList;

	std::vector<Coin*> m_coinList;

	int m_starFrameCount;
	int m_mapIndex;
	int m_rowIndex;

	bool m_isHostWorld;
	bool m_isWorldRunning;

	std::vector<NetPlayerData*> m_netPlayerDatas;
	uint16_t m_currentNetworkObjectID;
	std::map<uint16_t, NetObject*> m_netObjects;

	//Clock stuff
	double m_actualTime;
	double m_prevActualTime;

	double m_desiredTime;
	double m_prevDesiredTime;

	Sprite* m_spriteAttractScreen;
	Sprite* m_spriteGameOverScreen;
	Sprite* m_spriteVictoryScreen;
	Sprite* m_spriteStrikerRound;
	Sprite* m_spriteSpeedRound;
	Sprite* m_spriteSlayerRound;

	Sprite* m_currentWaveSprite;

	Sprite* m_background;

	AnimatedSprite* m_attractAnimScreen;
	AnimatedSprite* m_attractReadyAnimScreen;



	std::vector<uint16_t> m_netObjsToErase;



	Player* FindPlayerByIndex(uint8_t index);
	Player* AddPlayerAtIndex(uint8_t index);
	void RemovePlayerAtIndex(uint8_t index);

	void Update(float deltaSeconds);
	void UpdateHostTime(float deltaSeconds);
	void UpdateClientTime(float deltaSeconds);

	float GetActualNetDeltaSeconds();

	void StartRunningWorld(double netTime = 0);
	void OnUpdateNetTime(const NetSender &from, NetMessage &msg);
	bool IsActualTimeWithinDesiredThreshold();
	

	void UpdateCameraAndPlayer(float deltaSeconds);
	void UpdateEntities(float deltaSeconds);
	void UpdateBackGroundEntities(float deltaSeconds);
	void LoadMapEnts(float deltaSeconds);
	void EnableWaveSpriteForCurrentMap();
	void SpawnRow(MapRow row);
	void MakeStars();
	void Render();


	bool IsHostWorld();
	bool IsValidClientWorld();

	NetPlayerData* CreatePlayerData();
	void RequestMyPlayer(NetConnection* myConnection);
	NetPlayerData* FindPlayerDataByConnectionIndex(uint8_t connIndex);
	NetPlayerData* FindMyPlayerDataByNounce(uint32_t nounce);
	void RemovePlayerDataAtIndex(uint8_t connIndex);
	void RemoveAllNetObjectsOwnedByPlayerIndex(uint8_t playerIndex);


	uint16_t NetGenerateNetworkID();
	NetObject* NetSyncObject(void *obj_ptr, uint32_t const net_obj_type_id, NetPlayerData *owner);
	void NetDestroyObject(void *obj_ptr, uint32_t const net_obj_type_id, NetPlayerData *owner);
	void RegisterNetObject(NetObject * net_obj);
	void OnProcessObjectCreate(NetSender const &from, NetMessage &msg);
	void OnProcessObjectDestroy(NetSender const &from, NetMessage &msg);

	bool DoesObjectOfNetIDExist(uint16_t netID);
	void AddObjectToWorld(NetObject* obj, uint32_t typeID);
	void RemoveNetObjAndSignalOthers(uint16_t netID);
	void RemoveObjectFromWorld(NetObject* obj, uint32_t typeID);
	void SetupCoin(Coin * coin);
	void HostCreateCoin();
	void NetObjectUpdate(NetConnection *cp);
	void OnProcessObjectUpdateHost(NetSender const &from, NetMessage &msg);
	void OnProcessObjectUpdateClient(NetSender const &from, NetMessage &msg);
	void OnPlayerJoin(const NetSender &from, NetMessage &msg);
};
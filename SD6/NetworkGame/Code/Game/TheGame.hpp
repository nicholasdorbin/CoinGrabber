#pragma once
#include "Engine/MathUtils.hpp"
#include "Engine/Audio/Audio.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Game/World.hpp"
#include "Engine/Renderer/BitmapFontMeta.hpp"
#include "Engine/Math/Matrix4.hpp"
#include "Engine/Math/Matrix44.hpp"
#include "Engine/Core/EngineBuildConfig.hpp"
#include "Engine/Math/Quaternion.hpp"
#include "Game/GameState.hpp"
#include "Engine/Audio/Audio.hpp"


#include <string>

class TheGame;
class Command;
class Camera3D;
class GPUProgram;
class Mesh;
class Material;
class MeshRenderer;
class MeshBuilder;
class Command;
class Light;
class Skeleton;
class Motion;
class Quaternion;
class GameStateMachine;
class ParticleSystem;
class Job;
struct NetSender;
class NetConnection;
class NetMessage;
struct Event;
class NetPlayerData;

extern TheGame* g_theGame;
extern float g_timeScale;

void DrawDebugArrow(Command&);

class TheGame
{
public:
	TheGame::TheGame();
	~TheGame();


	bool m_canPause;
	bool m_isPaused;
	World* m_hostWorld;
	World* m_clientWorld;
	Texture* m_mask;
	Texture* m_noise;

	bool m_showAxis;
	int m_lightNum;
	int m_maxLights;
	int m_modelRenderState;
	bool m_showSkeleton;
	bool m_showModelMesh;
	float m_motionTime;
	int m_motionIndex;
	bool m_playAnimation;
	GameStateMachine m_state;


	Camera3D *m_camera;
	std::vector<Light> m_lights;
	Mesh *m_cubeMesh;
	Mesh *m_sphereMesh;
	Mesh *m_planeMesh;
	Mesh *m_modelMesh;
	MeshBuilder *m_modelMeshBuilder;
	MeshBuilder *m_meshBuilder;
	GPUProgram *m_programSolidColor;
	GPUProgram *m_programDot3;
	Material *m_materialWhite;
	Material *m_materialStone;
	Material *m_materialLava;
	Material *m_materialPlain;
	std::vector<MeshRenderer*> m_cubeMeshRenderer;
	std::vector<MeshRenderer*> m_lightMeshRenderer;

	MeshRenderer* m_modelRenderer;

	Skeleton* m_skeleton;
	std::vector< MeshRenderer* > m_jointMeshRenderer;
	std::vector<Motion*> m_motions;
	std::vector<mat44_fl> m_boneMatrices;
	int m_boneMatricesCount;
	float m_modelScale;

	//Camera stuff
	Quaternion m_startQuat;
	Quaternion m_endQuat;
	float m_quatSlerpTime;
	bool m_negateSlerpTime;
	bool m_enableCameraSystem;


	AudioSystem* m_audio;
	SoundID m_readyMusic;
	SoundID m_playingMusic;
	SoundID m_bossMusic;

	ParticleSystem* m_particleTest;

#ifdef MEMORY_TRACKER
	std::vector<std::string> m_verboseDataStrings;
	int m_memoryListLastIndexToShow;
#endif


	//NetGame Stuff
	float m_netClockUpdateTimer;
	float m_spawnTimer;





	void SetUpPerspectiveProjection() const;
	void Update(const float deltaSeconds);

	void HostUpdate(const float deltaSeconds);
	void ClientUpdate(const float deltaSeconds);

	void UpdateMemoryDebug(const float deltaSeconds);
	void UpdateCameraSystem(const float deltaSeconds);
	void Render(const float deltaSeconds) const;
	void DrawHUD() const;
	void DebugText(std::string text);
	void DrawCrosshair();
	void DrawMemoryDebug() const;
	void DrawSessionDebug() const;

	void ClearBuilderPointers(MeshBuilder* &meshBuilder, MeshRenderer* &modelRender, Mesh* &mesh);
	void PopulateJointMeshRenderer(Skeleton* skeleton);


	void RegisterGameMessagesAndEvents();
	void RegisterNetObjects();
	

	
	void OnConnectionJoin(Event* eventData);
	void OnConnectionLeave(Event* eventData);
	void OnConnectionJoinAccept(Event* eventData);
	void OnConnectionJoinRequestAccept(Event* eventData);
	void OnNetTick(Event* eventData);

	void OnUpdateReceiveHost(const NetSender &from, NetMessage &msg);
	void OnUpdateReceiveClient(const NetSender &from, NetMessage &msg);

	void SendAllConnectionsCurrentNetClock();
	
	//void OnConnectionJoin(NetConnection* cp);
	//void OnConnectionLeave(NetConnection* cp);
	//void OnNetTick(NetConnection* cp);

protected:
};

void TestJobFunction(Job* job);

void OnUpdateClientToHostReceive(const NetSender &from, NetMessage &msg);
void OnUpdateHostToClientReceive(const NetSender &from, NetMessage &msg);
void OnTestReliableReceive(const NetSender &from, NetMessage &msg);
void OnInOrderReliableReceive(const NetSender &from, NetMessage &msg);
void OnPlayerCreateRequest(const NetSender &from, NetMessage &request);
void OnPlayerJoin(const NetSender &from, NetMessage &msg);
void OnPlayerDeny(const NetSender &from, NetMessage &msg);
void OnNetObjCreate(const NetSender &from, NetMessage &msg);
void OnNetObjDestroy(const NetSender &from, NetMessage &msg);
void OnNetObjUpdate(const NetSender &from, NetMessage &msg);
void OnUpdateNetTime(const NetSender &from, NetMessage &msg);
#include "Game/TheGame.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/TheApp.hpp"
//#include "Game/GameCommon.hpp"
#include "Engine/Math/AABB3.hpp"

#include <time.h>
#include "Engine/Core/Time.hpp"
#include "Engine/Core/Console.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Game/GameCommon.hpp"

#include "Engine/Renderer/Shaders/GPUProgram.hpp"
#include "Engine/Renderer/Shaders/Mesh.hpp"
#include "Engine/Renderer/Shaders/MeshBuilder.hpp"
#include "Engine/Renderer/Shaders/Material.hpp"
#include "Engine/Core/Camera3D.hpp"
#include "Engine/Renderer/Shaders/Light.hpp"
#include "Engine/Tools/fbx.hpp"
#include "Engine/Renderer/Skeleton.hpp"
#include "Engine/Animation/Motion.hpp"
#include "Engine/Core/EngineBuildConfig.hpp"
#include "Engine/Core/Logger.hpp"
#include "Engine/Renderer/SpriteRenderer/Sprite.hpp"
#include "Engine/Renderer/ParticleRenderer/ParticleSystem.hpp"

//Network
#include "Engine/Core/Events/EventSystem.hpp"
#include "Game/Net/NetGameCommon.hpp"
#include "Engine/Net/Session/NetSession.hpp"
#include "Engine/Net/Session/NetConnection.hpp"
#include "Engine/Net/NetworkSystem.hpp"
#include "Game/Net/NetObject.hpp"
#include "Game/Net/NetObjectTypesSystem.hpp"





#ifdef MEMORY_TRACKER
#include "Engine/Core/Memory/MemoryAnalytics.hpp"
#endif

#pragma warning( disable : 4127 )  // conditional expression is constant

TheGame* g_theGame = nullptr;


const int MAX_MEM_VERBOSE_LINES = 5;
const float XHAIR_LENGTH = 15.f;
const std::string FONT_NAME = "Data/Fonts/ArialFont.fnt";
//const IntVector2 snapBackPos = IntVector2(800, 450);

const std::string TEST_FILE = "Data/Images/Test_StbiAndDirectX.png";

const std::string READY_MUSIC = "Data/Sounds/Music/06 - 3DRealms - Aliens Of Gold (Blake Stone Theme).wav";
const std::string PLAYING_MUSIC = "Data/Sounds/Music/03 - 3DRealms - Major Stryker Medley.wav";
const std::string BOSS_MUSIC = "Data/Sounds/Music/07 - 3DRealms - Alien Carnage (Halloween Harry).wav";


CONSOLE_COMMAND(fbx_list)
{
    if (args.m_argList.size() < 1) //Because I'm lazy
    {
        std::string filename = "Data/Models/SampleBox.fbx";
        FbxListScene(filename);
    }
    else
    {
        std::string filename = args.m_argList[0];
        FbxListScene(filename);
    }
        //return;
    

}

CONSOLE_COMMAND(fbx_load)
{
    args;
    /*
    if (args.m_argList.size() < 1) //Because I'm lazy
    {
        g_theGame->ClearBuilderPointers(g_theGame->m_modelMeshBuilder, g_theGame->m_modelRenderer, g_theGame->m_modelMesh);
    
        
        std::string filename = "Data/Models/samplebox.fbx";
        std::vector<MeshBuilder*> importMeshes;
        SceneImport* thisScene = FbxLoadSceneFromFile(filename, g_engineBasis, true, mat44_fl::identity);
        importMeshes = thisScene->meshes;
        g_theGame->m_modelScale = 1.f;
        //#TODO Make this NOT new off a MeshBuilder. Or at least deconstruct it.
        std::vector< Skeleton* > importSkeletons;
        std::vector<Motion*> importMotions;
        importSkeletons = thisScene->skeletons;
        importMotions = thisScene->motions;
        if (importSkeletons.size() > 0)
        {
            g_theGame->m_skeleton = importSkeletons[0];
            g_theGame->PopulateJointMeshRenderer(importSkeletons[0]);
        }
        if (importMotions.size() > 0)
        {
            g_theGame->m_motions = importMotions;
        }
        g_theGame->m_modelMeshBuilder = new MeshBuilder(importMeshes);
    }
    else if (args.m_argList.size() == 1)
    {
        g_theGame->ClearBuilderPointers(g_theGame->m_modelMeshBuilder, g_theGame->m_modelRenderer, g_theGame->m_modelMesh);

        std::string filename = "Data/Models/" + args.m_argList[0];
        std::vector<MeshBuilder*> importMeshes;
        SceneImport* thisScene = FbxLoadSceneFromFile(filename, g_engineBasis, true, mat44_fl::identity);
        importMeshes = thisScene->meshes;
        g_theGame->m_modelScale = 1.f;
        //#TODO Make this NOT new off a MeshBuilder. Or at least deconstruct it.
        std::vector< Skeleton* > importSkeletons;
        std::vector<Motion*> importMotions;
        importSkeletons = thisScene->skeletons;
        importMotions = thisScene->motions;
        g_theGame->m_modelMeshBuilder = new MeshBuilder(importMeshes);
        if (importSkeletons.size() == 1)
        {
            g_theGame->m_skeleton = importSkeletons[0];
            g_theGame->PopulateJointMeshRenderer(importSkeletons[0]);
        }
        if (importMotions.size() > 0)
        {
            g_theGame->m_motions = importMotions;
        }
        
    }
    else if (args.m_argList.size() == 2)
    {
        g_theGame->ClearBuilderPointers(g_theGame->m_modelMeshBuilder, g_theGame->m_modelRenderer, g_theGame->m_modelMesh);

        std::string filename = "Data/Models/" + args.m_argList[0];
        g_theGame->m_modelScale = stof(args.m_argList[1]);
        mat44_fl transform = mat44_fl::identity;
        MatrixMakeScale(&transform, g_theGame->m_modelScale);
        std::vector<MeshBuilder*> importMeshes;
        SceneImport* thisScene = FbxLoadSceneFromFile(filename, g_engineBasis, true, transform);
        importMeshes = thisScene->meshes;
        std::vector<Skeleton*> importSkeletons = thisScene->skeletons;
        std::vector<Motion*> importMotions;
        importMotions = thisScene->motions;

        if (importSkeletons.size() == 1)
        {
            g_theGame->m_skeleton = importSkeletons[0];
            g_theGame->PopulateJointMeshRenderer(importSkeletons[0]);
        }

        if (importMotions.size() > 0)
        {
            g_theGame->m_motions = importMotions;
        }
        g_theGame->m_modelMeshBuilder = new MeshBuilder(importMeshes);
    }
    //return;

    */
}

CONSOLE_COMMAND(mesh_save)
{
    if (g_theGame->m_modelMeshBuilder == nullptr)
    {
        return;
    }

    if (args.m_argList.size() < 1) //Because I'm lazy
    {
        std::string filename = "Data/Models/basicbox.mesh";
        g_theGame->m_modelMeshBuilder->WriteToFile(filename);
    }
    else
    {
        std::string filename = "Data/Models/" + args.m_argList[0];
        g_theGame->m_modelMeshBuilder->WriteToFile(filename);
    }
    //return;
}

CONSOLE_COMMAND(mesh_load)
{
  
    if (args.m_argList.size() < 1) //Because I'm lazy
    {
        g_theGame->ClearBuilderPointers(g_theGame->m_modelMeshBuilder, g_theGame->m_modelRenderer, g_theGame->m_modelMesh);
        g_theGame->m_modelMeshBuilder = new MeshBuilder();
        std::string filename = "Data/Models/basicbox.mesh";
        g_theGame->m_modelMeshBuilder->LoadFromFile(filename);
    }
    else
    {
        g_theGame->ClearBuilderPointers(g_theGame->m_modelMeshBuilder, g_theGame->m_modelRenderer, g_theGame->m_modelMesh);
        g_theGame->m_modelMeshBuilder = new MeshBuilder();
        std::string filename = "Data/Models/" + args.m_argList[0];
        g_theGame->m_modelMeshBuilder->LoadFromFile(filename);
    }
    //return;
}

CONSOLE_COMMAND(skel_save)
{
    if (g_theGame->m_skeleton == nullptr)
    {
        return;
    }

    if (args.m_argList.size() < 1) //Because I'm lazy
    {
        std::string filename = "Data/Models/basicbox.skel";
        g_theGame->m_skeleton->WriteToFile(filename);
    }
    else
    {
        std::string filename = "Data/Models/" + args.m_argList[0];
        g_theGame->m_skeleton->WriteToFile(filename);
    }
    //return;
}

CONSOLE_COMMAND(skel_load)
{

    if (args.m_argList.size() < 1) //Because I'm lazy
    {
        delete g_theGame->m_skeleton;
        g_theGame->m_skeleton = nullptr;
        g_theGame->m_skeleton = new Skeleton();
        std::string filename = "Data/Models/basicbox.skel";
        g_theGame->m_skeleton->LoadFromFile(filename);
        g_theGame->PopulateJointMeshRenderer(g_theGame->m_skeleton);
    }
    else
    {
        if (g_theGame->m_skeleton)
        {
            delete g_theGame->m_skeleton;
            g_theGame->m_skeleton = nullptr;
        }
        
        g_theGame->m_skeleton = new Skeleton();
        std::string filename = "Data/Models/" + args.m_argList[0];
        g_theGame->m_skeleton->LoadFromFile(filename);
        g_theGame->PopulateJointMeshRenderer(g_theGame->m_skeleton);
    }
    //return;
}

CONSOLE_COMMAND(motion_save)
{
    if (g_theGame->m_motions.size() == 0 || g_theGame->m_motions.size() <= (unsigned int) g_theGame->m_motionIndex)
    {
        return;
    }

    if (args.m_argList.size() < 1) //Because I'm lazy
    {
        std::string filename = "Data/Models/basicmotion.anim";
        g_theGame->m_motions[g_theGame->m_motionIndex]->WriteToFile(filename);
    }
    else
    {
        std::string filename = "Data/Models/" + args.m_argList[0];
        g_theGame->m_motions[g_theGame->m_motionIndex]->WriteToFile(filename);
    }

}

CONSOLE_COMMAND(motion_load)
{
    if (args.m_argList.size() < 1) //Because I'm lazy
    {
        Motion* newMotion = new Motion();
        std::string filename = "Data/Models/basicmotion.anim";
        newMotion->LoadFromFile(filename);
        g_theGame->m_motions.push_back(newMotion);
        g_theGame->m_motionIndex = g_theGame->m_motions.size() - 1;
    }
    else
    {

        Motion* newMotion = new Motion();
        std::string filename = "Data/Models/" + args.m_argList[0];
        newMotion->LoadFromFile(filename);
        g_theGame->m_motions.push_back(newMotion);
        g_theGame->m_motionIndex = g_theGame->m_motions.size() - 1;
    }
}


CONSOLE_COMMAND(toggle_skeleton)
{
    args;
    g_theGame->m_showSkeleton = !g_theGame->m_showSkeleton;
}

CONSOLE_COMMAND(toggle_model)
{
    args;
    g_theGame->m_showModelMesh = !g_theGame->m_showModelMesh;
}

CONSOLE_COMMAND(set_motion_index)
{
    if(args.m_argList.size() < 2)
        g_theGame->m_motionIndex = stoi(args.m_argList[0]);
}

CONSOLE_COMMAND(quat_test)
{
	args;
    g_theGame->m_enableCameraSystem = !g_theGame->m_enableCameraSystem;
}


#ifdef MEMORY_TRACKER
CONSOLE_COMMAND(memory_debug)
{
    args;
    g_DisplayMemoryDebug = !g_DisplayMemoryDebug;
}
#endif

CONSOLE_COMMAND(net_debug)
{
	args;
	g_DisplaySessionDebug = !g_DisplaySessionDebug;
}

CONSOLE_COMMAND(test_reliable) //<index> <count>
{
	args;
	if (args.m_argList.size() == 1)
	{
		uint8_t index = (uint8_t)atoi(args.m_argList[0].c_str());

		int count = 10;
		for (uint8_t i = 0u; i < count; i++)
		{
			NetConnection* thisConnection = g_netSession->GetConnection(index);
			if (thisConnection == nullptr /*|| thisConnection == g_netSession->m_myConnection*/)
				continue;
			NetMessage* reliableMessage = new NetMessage(eMessageID_NET_GAME_TEST_RELIABLE);

			reliableMessage->m_definition = g_netSession->FindDefinition(reliableMessage->m_messageType);

			reliableMessage->AssembleMessage();
			reliableMessage->FinalizeMessage();

			thisConnection->AddMessage(reliableMessage);

			thisConnection->SendPacket();
		}

	}
	else if (args.m_argList.size() > 1)
	{
		uint8_t index = (uint8_t)atoi(args.m_argList[0].c_str());

		int count = atoi(args.m_argList[1].c_str());

		for (uint8_t i = 0u; i < count; i++)
		{
			NetConnection* thisConnection = g_netSession->GetConnection(index);
			if (thisConnection == nullptr /*|| thisConnection == g_netSession->m_myConnection*/)
				continue;
			NetMessage* reliableMessage = new NetMessage(eMessageID_NET_GAME_TEST_RELIABLE);

			reliableMessage->m_definition = g_netSession->FindDefinition(reliableMessage->m_messageType);

			reliableMessage->AssembleMessage();
			reliableMessage->FinalizeMessage();

			thisConnection->AddMessage(reliableMessage);

			thisConnection->SendPacket();
		}
	}
	else
	{
		for (uint8_t i = 0u; i < 10; i++)
		{
			NetConnection* thisConnection = g_netSession->GetConnection(i);
			if (thisConnection == nullptr /*|| thisConnection == g_netSession->m_myConnection*/)
				continue;
			NetMessage* reliableMessage = new NetMessage(eMessageID_NET_GAME_TEST_RELIABLE);

			reliableMessage->m_definition = g_netSession->FindDefinition(reliableMessage->m_messageType);

			reliableMessage->AssembleMessage();
			reliableMessage->FinalizeMessage();

			thisConnection->AddMessage(reliableMessage);

			thisConnection->SendPacket();
		}
	}
	
	

}

CONSOLE_COMMAND(test_in_order) //<index> <count>
{
	args;
	if (args.m_argList.size() == 1)
	{
		uint8_t index = (uint8_t)atoi(args.m_argList[0].c_str());

		int count = 10;
		for (uint8_t i = 0u; i < count; i++)
		{
			NetConnection* thisConnection = g_netSession->GetConnection(index);
 			if (thisConnection == nullptr /*|| thisConnection == g_netSession->m_myConnection*/)
 				continue;
			NetMessage* reliableMessage = new NetMessage(eMessageID_NET_GAME_TEST_IN_ORDER);

			reliableMessage->m_definition = g_netSession->FindDefinition(reliableMessage->m_messageType);

			reliableMessage->AssembleMessage();
			reliableMessage->FinalizeMessage();

			thisConnection->AddMessage(reliableMessage);

			thisConnection->SendPacket();
		}

	}
	else if (args.m_argList.size() > 1)
	{
		uint8_t index = (uint8_t)atoi(args.m_argList[0].c_str());

		int count = atoi(args.m_argList[1].c_str());

		for (uint8_t i = 0u; i < count; i++)
		{
			NetConnection* thisConnection = g_netSession->GetConnection(index);
			if (thisConnection == nullptr /*|| thisConnection == g_netSession->m_myConnection*/)
 				continue;
			NetMessage* reliableMessage = new NetMessage(eMessageID_NET_GAME_TEST_IN_ORDER);

			reliableMessage->m_definition = g_netSession->FindDefinition(reliableMessage->m_messageType);

			reliableMessage->AssembleMessage();
			reliableMessage->FinalizeMessage();

			thisConnection->AddMessage(reliableMessage);

			thisConnection->SendPacket();
		}
	}
	else
	{
		for (uint8_t i = 0u; i < 10; i++)
		{
			NetConnection* thisConnection = g_netSession->GetConnection(i);
			if (thisConnection == nullptr /*|| thisConnection == g_netSession->m_myConnection*/)
 				continue;
			NetMessage* reliableMessage = new NetMessage(eMessageID_NET_GAME_TEST_IN_ORDER);

			reliableMessage->m_definition = g_netSession->FindDefinition(reliableMessage->m_messageType);

			reliableMessage->AssembleMessage();
			reliableMessage->FinalizeMessage();

			thisConnection->AddMessage(reliableMessage);

			thisConnection->SendPacket();
		}
	}



}

CONSOLE_COMMAND(make_coin)
{
	if (g_netSession == nullptr)
		return;

	if (g_netSession->m_isHost)
		g_theGame->m_hostWorld->HostCreateCoin();
}

TheGame::TheGame()
    : m_lightNum(2)
    , m_maxLights(16)
    , m_modelRenderState(0)
    , m_showSkeleton(true)
    , m_showModelMesh(true)
    , m_motionTime(0.f)
    , m_motionIndex(0)
    , m_playAnimation(true)
    , m_modelScale(1.0f)
    , m_quatSlerpTime(0.f)
    , m_negateSlerpTime(false)
    , m_enableCameraSystem(false)
	, m_state(STATE_PLAYING)
	, m_netClockUpdateTimer(0.f)
	, m_spawnTimer (0.f)
{
	g_spriteRenderSystem = new SpriteGameRenderer();
	m_audio = new AudioSystem();
	m_playingMusic = m_audio->CreateOrGetSound(PLAYING_MUSIC);
	m_readyMusic = m_audio->CreateOrGetSound(READY_MUSIC);
	m_bossMusic = m_audio->CreateOrGetSound(BOSS_MUSIC);


    m_startQuat = Quaternion(0.f, Vector3::FORWARD);
    m_endQuat = Quaternion(90.f, Vector3::UP);

#ifdef MEMORY_TRACKER
    m_memoryListLastIndexToShow = MAX_MEM_VERBOSE_LINES - 1;
#endif // MEMORY_TRACKER

    InitializeCommon();
	g_camera = Camera3D();
	g_camera.m_position = Vector3::ZERO;
	g_spriteRenderSystem->LoadAllSpriteResources();
	g_spriteRenderSystem->LoadAllAnimationSequences();
	g_ParticleEngine->LoadAllParticleSystemDefinitions();

	//m_particleTest = g_ParticleEngine->Create("smoke", LAYER_ATTRACT);
	//m_particleTest->Initialize();




    m_modelMeshBuilder = nullptr;
    m_modelRenderer = nullptr;
    m_modelMesh = nullptr;
    m_skeleton = nullptr;
    
	m_hostWorld = new World(true);
    m_clientWorld = new World();


    Console::RegisterCommand("draw", DrawDebugArrow );

	g_spriteRenderSystem->DisableAllButThisLayer(0);
	

	RegisterGameMessagesAndEvents();

	g_theConsole->ExecuteCommand("net_session_start");
	g_theConsole->ExecuteCommand("net_debug");

	
	/*
	Job* job = g_theJobSystem->JobCreate(GENERIC, TestJobFunction);

	job->JobWrite<float>(1.f);
	job->JobWrite<int>(42);


	float testFloat = job->JobRead<float>();
	int testInt = job->JobRead<int>();
	*/
}


TheGame::~TheGame()
{

	delete m_clientWorld;
	delete m_hostWorld;
	// 	delete m_mask;
	// 	delete m_noise;
	// 	delete m_camera;
	// 	delete m_cubeMesh;
	// 	
	// 	delete m_sphereMesh;
	// 	delete m_planeMesh;
	// 	delete m_modelMesh;
	// 	delete m_modelMeshBuilder;
	// 	delete m_meshBuilder;
	// 	delete m_programSolidColor;
	// 	delete m_programDot3;
	// 	delete m_materialWhite;
	// 	delete m_materialStone;
	// 	delete m_materialLava;
	// 	delete m_materialPlain;




// 	for each (NetPlayerData* playerData  in m_netPlayerDatasHost)
// 	{
// 		delete playerData;
// 	}

	for each (MeshRenderer* meshRenderer in m_cubeMeshRenderer)
	{
		delete meshRenderer;
	}

	for each (MeshRenderer* meshRenderer in m_lightMeshRenderer)
	{
		delete meshRenderer;
	}


	delete m_modelRenderer;

	delete m_skeleton;
	for each (MeshRenderer* meshRenderer in m_jointMeshRenderer)
	{
		delete meshRenderer;
	}

	for each (Motion* motion in m_motions)
	{
		delete motion;
	}

	delete m_audio;
	
	DestroyCommon();
	//delete m_particleTest;
}


void TheGame::SetUpPerspectiveProjection() const
{
    float aspect = (16.f / 9.f);
    //float fovDegreesHorizontal = 90.f;
    float fovDegreesVertical = 70.f;
    float zNear = 0.1f;
    float zFar = 100.f;

    g_theRenderer->SetPerspective(fovDegreesVertical, aspect, zNear, zFar);
}

void TheGame::Update(const float deltaSeconds)
{
	g_theProfiler->StartProfilerSample("GameUpdate");
    g_engineTotalTime += deltaSeconds;
	m_spawnTimer += deltaSeconds;
    if (g_theApp->m_inputSystem.IsKeyJustPressed(0xc0) && g_theApp->m_inputSystem.IsKeyDown(0xc0)) //why tho ~
    {
        g_theConsole->m_isOpen = !g_theConsole->m_isOpen;
    }
    if (!g_theConsole->m_isOpen)
    {

		if (g_theApp->m_inputSystem.IsKeyPressed('P'))
		{
			g_theConsole->ExecuteCommand("make_coin");
		}

		if (m_spawnTimer > 5.f)
		{
			m_spawnTimer = 0.f;
			if (g_netSession->m_isHost)
				g_theGame->m_hostWorld->HostCreateCoin();
		}

		//m_particleTest->Update(deltaSeconds);

       // UpdateCameraSystem(deltaSeconds);
		if (m_state.GetState() == STATE_ATTRACT_SCREEN)
		{
			g_spriteRenderSystem->DisableAllButThisLayer(eGame_SpriteLayers::LAYER_ATTRACT);
			g_spriteRenderSystem->EnableLayer(eGame_SpriteLayers::LAYER_BACKGROUND);

			if (g_theApp->m_inputSystem.IsKeyJustPressed(VK_RETURN) || g_theApp->m_controllers[0].IsButtonPressedOnce(XBOX_START))
			{
				m_state.SetState(STATE_READY);
				m_clientWorld->m_worldTimer = 0.f;
				//g_spriteRenderSystem->EnableAllButThisLayer(eGame_SpriteLayers::LAYER_ATTRACT);
			}
		}

		if (m_state.GetState() == STATE_READY)
		{
			if (m_clientWorld->m_worldTimer > 1.5f)
			{
				m_state.SetState(STATE_PLAYING);
				m_clientWorld->m_worldTimer = 0.f;
				m_clientWorld->EnableWaveSpriteForCurrentMap();
				g_spriteRenderSystem->EnableAllButThisLayer(eGame_SpriteLayers::LAYER_ATTRACT);
				
				
			}
		}

    
		if (g_theApp->m_controllers[0].IsButtonDown(XBOX_BACK))
		{
			g_isQuitting = true;
		}

    }
    else
    {
        g_theConsole->Update(deltaSeconds);
		
    }
    

	if (g_netSession->m_isHost)
	{
		HostUpdate(deltaSeconds);
	}

	if (m_clientWorld->m_myPlayer != nullptr)
	{
		ClientUpdate(deltaSeconds);
	}
	


    g_theApp->m_inputSystem.UpdateKeyJustPressedState();
    g_theConsole->m_inputSystem.UpdateKeyJustPressedState();
    g_theRenderer->UpdateRenderCommands(deltaSeconds);
	g_theProfiler->EndProfilerSample();
}

void TheGame::HostUpdate(const float deltaSeconds)
{

	m_hostWorld->Update(deltaSeconds);
	m_netClockUpdateTimer += deltaSeconds;
	if (m_netClockUpdateTimer >= GAME_NET_TIME_UPDATE_TICKRATE)
	{
		m_netClockUpdateTimer = 0.f;
		SendAllConnectionsCurrentNetClock();
	}
}

void TheGame::ClientUpdate(const float deltaSeconds)
{
	m_clientWorld->Update(deltaSeconds);
	m_audio->Update(deltaSeconds);
	static float m_age;
	m_age += deltaSeconds;


	UpdateMemoryDebug(deltaSeconds);
}

void TheGame::UpdateMemoryDebug(const float deltaSeconds)
{
	deltaSeconds;
    #ifdef MEMORY_TRACKER
    if (!g_theConsole->m_isOpen)
    {
        if (g_theApp->m_inputSystem.IsKeyDown(VK_NEXT) && g_theApp->m_inputSystem.IsKeyJustPressed(VK_NEXT)) // PGDOWN
        {
            if ((unsigned int)m_memoryListLastIndexToShow < m_verboseDataStrings.size())
                m_memoryListLastIndexToShow++;
        }
        else if (g_theApp->m_inputSystem.IsKeyDown(VK_PRIOR) && g_theApp->m_inputSystem.IsKeyJustPressed(VK_PRIOR)) // PGUP
        {
            if (m_memoryListLastIndexToShow >= MAX_MEM_VERBOSE_LINES)
                m_memoryListLastIndexToShow--;
        }
    }
    

    if (g_engineTotalTime - g_timestampMemory > 1)
    {
        g_timestampMemory = g_engineTotalTime;
        g_BytesAllocedLastTick = g_BytesAllocedPerSec;
        g_BytesFreedLastTick = g_BytesFreedPerSec;

        g_BytesAllocedPerSec = 0;
        g_BytesFreedPerSec = 0;
    }

    if (MEMORY_TRACKER == 1)
    {


        UpdateCallstackMemoryHashMap();

        m_verboseDataStrings.clear();

        //Iterate thru the callstacks
        for (auto callstackItem : g_callstackHashMap)
        {
            //Show the file line, Alloc Count, Total Size of the alloc
            unsigned char* fileName = callstackItem.second.filename;
            uint32_t fileLine =  callstackItem.second.fileline;

            unsigned int numAllocs = callstackItem.second.alloc_count;
            unsigned int totalSize = callstackItem.second.total_bytes;

            std::string thisDebugStr = Stringf("File: %s, Line: #%u ___ Amount: %u, Size: %u ", fileName, fileLine, numAllocs, totalSize);

            m_verboseDataStrings.push_back(thisDebugStr);
        }

        if ((size_t)m_memoryListLastIndexToShow > m_verboseDataStrings.size() - 1)
            m_memoryListLastIndexToShow = m_verboseDataStrings.size() - 1;
    }
    #endif
}

void TheGame::UpdateCameraSystem(const float deltaSeconds)
{
    if (m_negateSlerpTime)
    {
        m_quatSlerpTime -= deltaSeconds;
    }
    else
    {
        m_quatSlerpTime += deltaSeconds;
    }
    

    if (m_quatSlerpTime > 1.f) {
        m_negateSlerpTime = true;
        //m_dt *= -1.f;
        m_quatSlerpTime = 1.f - (m_quatSlerpTime - 1.f);
    }
    else if (m_quatSlerpTime < 0.f) {
        m_negateSlerpTime = false;
        //m_dt *= -1.f;
        m_quatSlerpTime = -m_quatSlerpTime;
    }

    Quaternion intermidiateQuat = SlerpQuats(m_startQuat, m_endQuat, m_quatSlerpTime);

    if (m_enableCameraSystem)
    {
        intermidiateQuat.ToEulerAngles(g_camera.m_orientation);
        g_camera.FixAndClampAngles();
    }
}

void TheGame::Render(const float deltaSeconds) const
{
	deltaSeconds;


	//g_theRenderer->DrawAABB(m_world->m_player.m_bounds, Rgba::RED);

	g_theProfiler->StartProfilerSample("SpriteRender");
    g_spriteRenderSystem->Render();
	g_theProfiler->EndProfilerSample();
// 	static BitmapFontMeta* bitmapFont = BitmapFontMeta::CreateOrGetFont(FONT_NAME);
// 	Rgba playerHealthColor = Rgba::WHITE;
// 
// 	std::string clockStr = "Current: " + Stringf("%f", (float)g_systemClock->m_currentTime);
// 	std::string clockStr2 = "Delta: " + Stringf("%f", (float)g_systemClock->GetDelta());
// 	g_theRenderer->DrawTextMeta2D(Vector3(20.f, 80.f, 0.f), 0.8f,
// 		clockStr,
// 		playerHealthColor, bitmapFont);
// 	g_theRenderer->DrawTextMeta2D(Vector3(20.f, 50.f, 0.f), 0.8f,
// 		clockStr2,
// 		playerHealthColor, bitmapFont);
	if (m_state.GetState() == STATE_PLAYING)
	{
		//DrawHUD();
	}
	//g_theRenderer->ApplyRenderState(RenderState::BASIC_2D);

	m_clientWorld->Render();

	g_theProfiler->StartProfilerSample("DebugRender");
    g_theRenderer->SetDepthTest(false);
    g_theRenderer->SetOrtho(Vector2(0.f, 1600.f), Vector2(0.f, 900.f));
    DrawMemoryDebug();
	DrawSessionDebug();
	g_theProfiler->EndProfilerSample();
	g_theProfiler->StartProfilerSample("ConsoleRender");
    g_theConsole->Render();
	g_theProfiler->EndProfilerSample();
	
	//g_theRenderer->DrawTexturedAABB(m_world->m_player.m_bounds , 0, Vector2(0.f, 0.f), Vector2(1.f, 1.f), Rgba::RED);
    


    //g_theGame->DrawCrosshair();
}

void TheGame::DrawHUD() const
{
	static BitmapFontMeta* bitmapFont = BitmapFontMeta::CreateOrGetFont(FONT_NAME);
	Rgba playerHealthColor = Rgba::GREEN;
	float percentHealth = m_clientWorld->m_myPlayer->GetPercentHealthRemaining();
	if (percentHealth <= 50.f)
	{
		playerHealthColor = Rgba::YELLOW;
	}
	if (percentHealth <= 25.f)
	{
		playerHealthColor = Rgba::RED;
	}
	std::string playerHealthStr = "Shields: " + Stringf("%.0f", percentHealth) + "%";
	g_theRenderer->DrawTextMeta2D(Vector3(20.f, 50.f, 0.f), 0.8f,
		playerHealthStr,
		playerHealthColor, bitmapFont);
}

void TheGame::DebugText(std::string text)
{
    //g_theRenderer->DrawText2D(Vector2(20.f, 800.f), text, 15.f, Rgba::WHITE, m_font);
}

void TheGame::DrawCrosshair()
{
    Rgba color = Rgba::WHITE;
    Vector2 xhairBottom = Vector2((float)snapBackPos.x, (float)snapBackPos.y - XHAIR_LENGTH);
    Vector2 xhairTop = Vector2((float)snapBackPos.x, (float)snapBackPos.y + XHAIR_LENGTH);
    Vector2 xhairLeft = Vector2((float)snapBackPos.x - XHAIR_LENGTH, (float)snapBackPos.y);
    Vector2 xhairRight = Vector2((float)snapBackPos.x + XHAIR_LENGTH, (float)snapBackPos.y);

    g_theRenderer->SetInverseDestBlend();
    g_theRenderer->DrawLine(xhairBottom, xhairTop, color.m_red, color.m_green, color.m_blue, color.m_alpha, 2.f);
    g_theRenderer->DrawLine(xhairLeft, xhairRight, color.m_red, color.m_green, color.m_blue, color.m_alpha, 2.f);
    g_theRenderer->SetAlphaBlend();
}

void TheGame::DrawMemoryDebug() const
{
	static BitmapFontMeta* bitmapFont = BitmapFontMeta::CreateOrGetFont(FONT_NAME);
#ifdef MEMORY_TRACKER



    if (g_DisplayMemoryDebug)
    {
        
        std::string currentAllocationText = "Current Allocs: " + std::to_string(g_NumberOfAllocations);
        std::string totalAllocationText = " Total Bytes: " + std::to_string(g_TotalAllocated);
        std::string maxAllocationText = " Max Bytes: " + std::to_string(g_MaxAllocated);
        std::string allocationLastTickText = " Bytes Alloc Last Tick: " + std::to_string(g_BytesAllocedLastTick);
        std::string freedLastTickText = " Bytes Freed Last Tick: " + std::to_string(g_BytesFreedLastTick);
        int memDiff = g_BytesAllocedLastTick - g_BytesFreedLastTick;
        std::string avgMemChangeText = " Mem Change Last Tick: " + std::to_string(memDiff);



        g_theRenderer->DrawTextMeta2D(Vector3(20.f, g_theApp->WINDOW_PHYSICAL_HEIGHT - 20.f, 0.f), 0.3f,
            currentAllocationText + totalAllocationText + maxAllocationText + allocationLastTickText + freedLastTickText + avgMemChangeText,
            Rgba::WHITE, bitmapFont);

        if (MEMORY_TRACKER == 1)
        {
            std::vector<std::string> linesToShow;
            linesToShow.resize(5);
            for each (std::string str in linesToShow)
            {
                str = "";
            }
            

            int indexToStartAt = 0;
            
            int indexToEndAt = m_verboseDataStrings.size() - 1;
            if (m_memoryListLastIndexToShow >= MAX_MEM_VERBOSE_LINES)
            {
                indexToEndAt = m_memoryListLastIndexToShow;
                indexToStartAt = indexToEndAt - (MAX_MEM_VERBOSE_LINES - 1);
            }



            float offsetY = g_theApp->WINDOW_PHYSICAL_HEIGHT - 60.f;
            std::string amountOfLinesStr = Stringf("Showing CallStack Lines %i - %i out of %i", indexToStartAt + 1, indexToStartAt + MAX_MEM_VERBOSE_LINES, g_callstackHashMap.size());
            g_theRenderer->DrawTextMeta2D(Vector3(20.f, offsetY + 20.f, 0.f), 0.3f,
                amountOfLinesStr,
                Rgba::WHITE, bitmapFont);
            for (int i = 0; i < MAX_MEM_VERBOSE_LINES; i++)
            {
                if ((size_t)(indexToStartAt + i) >= m_verboseDataStrings.size())
                    break;

                linesToShow[i] = m_verboseDataStrings[indexToStartAt + i];
                g_theRenderer->DrawTextMeta2D(Vector3(20.f, offsetY - (i * 20.f), 0.f), 0.3f,
                    linesToShow[i],
                    Rgba::WHITE, bitmapFont);
            }

            
        }
    }
#endif // MEMORY_TRACKER


#ifdef PROFILER
	if (g_theProfiler->IsEnabled())
	{
		double frameTime = g_theProfiler->GetTimeOfPrevFrame();
		int framesPerSec = (int)floor(1 / frameTime);
		std::string frameTimeStr = Stringf("Frame Time: %f", frameTime);
		std::string fpsStr = Stringf("FPS: %i", framesPerSec);
		g_theRenderer->DrawTextMeta2D(Vector3(20.f, 80.f, 0.f), 0.5f,
			frameTimeStr,
			Rgba::WHITE, bitmapFont);
		g_theRenderer->DrawTextMeta2D(Vector3(20.f, 120.f, 0.f), 0.5f,
			fpsStr,
			Rgba::WHITE, bitmapFont);


		float offsetY = g_theApp->WINDOW_PHYSICAL_HEIGHT - 60.f;
		if (g_theProfiler->m_showLiveUpdate)
		{
			std::vector<std::string> sampleLines = g_theProfiler->GetLiveFrameStrings();
			for (size_t i = 0; i < sampleLines.size()-1; i++)
			{
	

				g_theRenderer->DrawTextMeta2D(Vector3(20.f, offsetY - (i * 20.f), 0.f), 0.3f,
					sampleLines[i],
					Rgba::WHITE, bitmapFont);
			}
		}
		
	}
#endif
}

void TheGame::DrawSessionDebug() const //net_debug
{
	if (!g_DisplaySessionDebug)
		return;
	static BitmapFontMeta* bitmapFont = BitmapFontMeta::CreateOrGetFont(FONT_NAME);
	//Time of last Sent to any connection
	//Time of last Recv from any connection
	//Session State
	//If Socket Bound, display bound addr
	//Display current artifical lag & loss

	std::string sessionStateText = g_netSession->GetSessionStateName();
	std::string timestampText = " Session Sent Time: " + std::to_string(g_netSession->GetLastSentTimeStamp()) + " Recv Time: " + std::to_string(g_netSession->GetLastRecvTimeStamp());
	std::string socketStr = " ";
	if (g_netSession->m_sessionSocket.IsBound())
	{
		socketStr += g_networkSystem->SockAddrToString((sockaddr*)&g_netSession->m_sessionSocket.m_socket.m_addrIn); 
		//socketStr += " : ";
		//socketStr += (g_netSession->m_sessionSocket.m_socket.m_port);
	}


	std::string lagLossStr = " Lag: " + std::to_string(g_netSession->m_sessionSocket.m_lagRange.min) + "-" + std::to_string(g_netSession->m_sessionSocket.m_lagRange.max) 
		+ " Loss: " + std::to_string(g_netSession->m_sessionSocket.m_dropRate);


	std::string sessionText = sessionStateText + timestampText + socketStr + lagLossStr;

	std::string sessionIP = "No IP";
	if (g_netSession->IsRunning())
	{
		sessionIP = g_networkSystem->SockAddrToString((sockaddr*)&g_netSession->m_sessionSocket.m_socket.m_addrIn);
	}

	g_theRenderer->DrawTextMeta2D(Vector3(20.f, g_theApp->WINDOW_PHYSICAL_HEIGHT - 20.f, 0.f), 0.3f,
		sessionText,
		Rgba::WHITE, bitmapFont);
	g_theRenderer->DrawTextMeta2D(Vector3(20.f, g_theApp->WINDOW_PHYSICAL_HEIGHT - 40.f, 0.f), 0.3f,
		"     @" + sessionIP,
		Rgba::GREEN, bitmapFont);

	//Display all current connections
	//For each connection:
	/*
		//Show if it's my connection
		//Connection Index
		//Conn addr and port
		//guid
		//Time of last sent to this conn
		//Time of last recv from this conn
		//last ack sent to this conn
		//display most recent confirmed ack and bitfield
	*/
	float offsetY = g_theApp->WINDOW_PHYSICAL_HEIGHT - 60.f;
	int lineIndex = 0;
	for (uint8_t i = 0u; i < MAX_CONNECTIONS; i++)
	{
		NetConnection* thisConnection = g_netSession->GetConnection(i);
		if (thisConnection == nullptr)
			continue;

		std::string myConnectionMarker = "";
		if (thisConnection == g_netSession->m_myConnection)
		{
			myConnectionMarker = "*";
		}

		if (thisConnection->m_index == 0)//#TODO Find better way to determine if host. Currently hard coded to first index.
		{
			myConnectionMarker += "[H] ";
		}
		else
		{
			myConnectionMarker += "[C] ";
		}

		std::string connIndexStr = "[" + std::to_string(thisConnection->m_index) + "]";

		std::string connAddrPortStr = g_networkSystem->SockAddrToString((sockaddr*)&thisConnection->m_address);

		std::string connState = " " + thisConnection->GetConnectionStateName() + " ";

		std::string guidStr = thisConnection->m_guid; 

		std::string timestampStr = " Sent: " + std::to_string(thisConnection->m_lastSentTimestamp) + " Recv: " + std::to_string(thisConnection->m_lastReceivedTimestamp);


		std::string ackStr = " LastAckSent: " + std::to_string(thisConnection->m_nextSentAckID - 1);

		std::string confAckStr = "Confirmed Ack & BF: " + std::to_string(thisConnection->m_highestReceivedAckID) 
			+ ", " + std::to_string(thisConnection->m_prevHighestReceivedAcksBitfield);

		float rtt = (float)thisConnection->m_rtt;

		std::string rttStr = Stringf(" RTT: %.3f", rtt);

		std::string netClockStr = "";

		if (i == 0)
		{
			netClockStr = Stringf(" H_Clock A: %.3f C_Clock A: %.3f D: %.3f", m_hostWorld->m_actualTime, m_clientWorld->m_actualTime, m_clientWorld->m_desiredTime);
		}
		else
		{
			netClockStr = Stringf(" SClock A: %.3f D: %.3f", m_clientWorld->m_actualTime, m_clientWorld->m_desiredTime);
		}
		std::string connectionStr = myConnectionMarker + connIndexStr + connAddrPortStr + connState + guidStr + timestampStr + ackStr + confAckStr + rttStr + netClockStr;

		g_theRenderer->DrawTextMeta2D(Vector3(20.f, offsetY - (lineIndex * 20.f), 0.f), 0.3f,
			connectionStr,
			Rgba::WHITE, bitmapFont);
		lineIndex++;
	}
}

void TheGame::ClearBuilderPointers(MeshBuilder* &meshBuilder, MeshRenderer* &modelRender, Mesh* &mesh)
{
    delete meshBuilder;
    meshBuilder = nullptr;

    delete modelRender;
    modelRender = nullptr;

    delete mesh;
    mesh = nullptr;
}

void TheGame::PopulateJointMeshRenderer(Skeleton* skeleton)
{
    if (m_jointMeshRenderer.size() > 0)
    {
        m_jointMeshRenderer.clear();
    }

    for (unsigned int jointIndex = 0; jointIndex < skeleton->m_jointNames.size(); ++jointIndex)
    {
        //#TODO Change to new matrix
// 		m_jointMeshRenderer.push_back(new MeshRenderer(m_sphereMesh, m_materialStone,
// 			Transform(skeleton->GetJointPosition(jointIndex), mat44_fl::identity,
// 				Vector3(0.1f, 0.1f, 0.1f))));
    }
}

void TheGame::RegisterGameMessagesAndEvents()
{
	EventSystem* eventSys = EventSystem::GetInstance();
	eventSys->RegisterEvent<TheGame, &TheGame::OnNetTick>(std::string("onNetTick"), this);
	eventSys->RegisterEvent<TheGame, &TheGame::OnConnectionJoin>(std::string("onConnectionJoin"), this);
	eventSys->RegisterEvent<TheGame, &TheGame::OnConnectionLeave>(std::string("onConnectionLeave"), this);
	eventSys->RegisterEvent<TheGame, &TheGame::OnConnectionJoinAccept>(std::string("onConnectionJoinAccept"), this);//onConnectionJoinRequestAccept
	eventSys->RegisterEvent<TheGame, &TheGame::OnConnectionJoinRequestAccept>(std::string("onConnectionJoinRequestAccept"), this);

	g_netSession->RegisterMessage(eMessageID_NET_GAME_UPDATE_HOST_TO_CLIENT, "updateH2C", OnUpdateHostToClientReceive);
	g_netSession->RegisterMessage(eMessageID_NET_GAME_UPDATE_CLIENT_TO_HOST, "updateC2H", OnUpdateClientToHostReceive);
	g_netSession->RegisterMessage(eMessageID_NET_GAME_TEST_RELIABLE, "testReliable", OnTestReliableReceive, 0, eOptionFlagID_RELIABLE); 
	g_netSession->RegisterMessage(eMessageID_NET_GAME_TEST_IN_ORDER, "testInOrder", OnInOrderReliableReceive, 0, eOptionFlagID_RELIABLE | eOptionFlagID_INORDER); 
	g_netSession->RegisterMessage(eMessageID_NET_GAME_REQUEST_PLAYER, "playerRequest", OnPlayerCreateRequest, 0, eOptionFlagID_RELIABLE | eOptionFlagID_INORDER);
	g_netSession->RegisterMessage(eMessageID_NET_GAME_PLAYER_JOINED, "playerJoined", OnPlayerJoin, 0, eOptionFlagID_RELIABLE | eOptionFlagID_INORDER);
	g_netSession->RegisterMessage(eMessageID_NET_GAME_PLAYER_DENY, "playerDeny", OnPlayerDeny);


	//NetObjects
	g_netSession->RegisterMessage(eMessageID_NET_GAME_OBJECT_CREATE, "netObjectCreate", OnNetObjCreate, 0, eOptionFlagID_RELIABLE | eOptionFlagID_INORDER);
	g_netSession->RegisterMessage(eMessageID_NET_GAME_OBJECT_DESTROY, "netObjectDestroy", OnNetObjDestroy, 0, eOptionFlagID_RELIABLE | eOptionFlagID_INORDER);
	g_netSession->RegisterMessage(eMessageID_NET_GAME_OBJECT_UPDATE, "netObjectUpdate", OnNetObjUpdate, 0, eOptionFlagID_RELIABLE | eOptionFlagID_INORDER);

	//NetTime
	//eMessageID_NET_GAME_NET_TIME_UPDATE
	g_netSession->RegisterMessage(eMessageID_NET_GAME_NET_TIME_UPDATE, "netTimeUpdate", OnUpdateNetTime, 0, eOptionFlagID_RELIABLE | eOptionFlagID_INORDER);
}


void TheGame::OnConnectionJoin(Event* eventData)
{
	//CreateObject(cp->get_index());
	SessionEvent* gameConnectionEventData = (SessionEvent*)eventData;
	

	NetConnection* conn = gameConnectionEventData->connection;


	// Am I the host of this session?
	if (g_netSession->m_isHost)
	{
		//Send all the PlayerDatas we know about to the fella that just joined

// 		NetMessage joinMessage = NetMessage(eMessageID_NET_GAME_PLAYER_JOINED);
// 		joinMessage.m_definition = g_netSession->FindDefinition(joinMessage.m_messageType);
// 		joinMessage.WriteToMessage<uint8_t>(&conn->m_index);
// 		joinMessage.FinalizeMessage();
// 
// 		g_netSession->SendMessageToAllConnectionsExcluding(joinMessage, conn->m_index);

		if (conn == g_netSession->m_myConnection)
		{
			m_hostWorld->StartRunningWorld();
			m_clientWorld->RequestMyPlayer(conn);
			//RequestMyPlayer(conn);
		}
	}

	if (conn == g_netSession->m_myConnection)
	{
		//RequestMyPlayer(conn);
	}
	/*
	NetSession *session = cp->session;
	if (session->am_i_host()) {
		foreach(NetPlayerData I Know About) {
			Send NetPlayerData to this connection.
		}
	}
	*/
	/*

	//Old stuff
	
	Player* connPlayer = m_world->FindPlayerByIndex(conn->m_index);

	if (connPlayer != nullptr)
	{
		g_theConsole->ConsolePrint("Trying to add player on existing ID.", Rgba::RED);

		return;
	}
	
	

	//ConnPlayer is nullptr, add a player

	Player* newPlayer = m_world->AddPlayerAtIndex(conn->m_index);

	if (g_netSession->m_myConnection != nullptr)
	{
		if (conn->m_index == g_netSession->m_myConnection->m_index)
		{
			m_world->m_myPlayer = newPlayer;
		}
	}
	
	*/
	g_theConsole->ConsolePrint("Hey boss someone joined");
	
}


void TheGame::OnConnectionLeave(Event* eventData)
{
	//Destroy that shit;
	SessionEvent* gameConnectionEventData = (SessionEvent*)eventData;
	gameConnectionEventData;

	if (g_netSession->m_isHost)
	{
		m_hostWorld->RemovePlayerAtIndex(gameConnectionEventData->index);
		//#TODO Handle deleting net objects on host and client
	}

	m_clientWorld->RemovePlayerAtIndex(gameConnectionEventData->index);
	g_theConsole->ConsolePrint("Hey boss someone left");
}

void TheGame::OnConnectionJoinAccept(Event* eventData)
{
	SessionEvent* gameConnectionEventData = (SessionEvent*)eventData;
	gameConnectionEventData;

	//#TODO Get timestamp from msg
	NetMessage* msg = gameConnectionEventData->msg;


	double netTimeStamp = 0;
	msg->ReadFromMessage<double>(&netTimeStamp);
	double halfRTT = gameConnectionEventData->connection->m_rtt / 2.f;
	m_clientWorld->StartRunningWorld(netTimeStamp + halfRTT);
	

	m_clientWorld->RequestMyPlayer(g_netSession->m_myConnection);
	//RequestMyPlayer(g_netSession->m_myConnection);

}


void TheGame::OnConnectionJoinRequestAccept(Event* eventData) //Write host timestamp into this message
{
	SessionEvent* gameConnectionEventData = (SessionEvent*)eventData;
	if (m_hostWorld->m_isWorldRunning)
	{
		NetMessage* msg = gameConnectionEventData->msg;
		msg->WriteToMessage<double>(&m_hostWorld->m_actualTime);
	}
}

void TheGame::OnNetTick(Event* eventData)
{
	//Send to everyone who isn't me my player's location

	SessionEvent* gameConnectionEventData = (SessionEvent*)eventData;
	
	NetConnection* conn = gameConnectionEventData->connection;


	if (g_netSession->m_isHost)
	{
		for each (Player* player in m_hostWorld->m_players)
		{
			NetMessage* updateMessage = new NetMessage(eMessageID_NET_GAME_UPDATE_HOST_TO_CLIENT);

			updateMessage->m_definition = g_netSession->FindDefinition(updateMessage->m_messageType);
			updateMessage->WriteToMessage<uint8_t>(&player->m_ownerNetIdx);
			updateMessage->WriteToMessage<Vector2>(&player->m_pos);
			updateMessage->WriteToMessage<uint8_t>(&player->m_direction);
			if (player->m_score != 0)
			{
				int breakHere = 0;
			}
			updateMessage->WriteToMessage<uint16_t>(&player->m_score);
			updateMessage->FinalizeMessage();

			conn->AddMessage(updateMessage);
		}
		m_hostWorld->NetObjectUpdate(conn);

	}

	if (m_clientWorld->m_myPlayer != nullptr)
	{
		if (conn->m_index == 0)
		{
			//Write my Player and send it.
			NetMessage* updateMessage = new NetMessage(eMessageID_NET_GAME_UPDATE_CLIENT_TO_HOST);

			updateMessage->m_definition = g_netSession->FindDefinition(updateMessage->m_messageType);
			updateMessage->WriteToMessage<uint8_t>(&m_clientWorld->m_myPlayer->m_ownerNetIdx);
			updateMessage->WriteToMessage<Vector2>(&m_clientWorld->m_myPlayer->m_pos);
			updateMessage->WriteToMessage<uint8_t>(&m_clientWorld->m_myPlayer->m_direction);
			updateMessage->FinalizeMessage();

			conn->AddMessage(updateMessage);
		}
		m_clientWorld->NetObjectUpdate(conn);
	}



}

void TheGame::OnUpdateReceiveHost(const NetSender &from, NetMessage &msg)
{
	NetConnection* conn = from.connection;
	msg.ResetOffset();
	msg.m_offset = msg.GetHeaderSize();

	if (conn == nullptr)
	{
		return;
	}
	uint8_t playerIndex;
	msg.Read<uint8_t>(&playerIndex);
	Player* connPlayer = g_theGame->m_hostWorld->FindPlayerByIndex(playerIndex);

	//	ASSERT_OR_DIE(connPlayer != nullptr, "Error: Have a connection with no attached player pointer");
	if (connPlayer == nullptr)
	{
		return;
	}

	

	
	Vector2 newPos = Vector2::ZERO;
	eGame_PlayerDirection newDir = eGame_PlayerDirection_NORTH;

	msg.Read<Vector2>(&newPos);
	msg.Read<uint8_t>(&newDir);

	connPlayer->m_direction = newDir;
	connPlayer->m_desiredPos = newPos;
	connPlayer->LerpToDesiredPos();
	//connPlayer->MoveAndUpdateBounds(newPos);
}

void TheGame::OnUpdateReceiveClient(const NetSender &from, NetMessage &msg)
{
	NetConnection* conn = from.connection;
	msg.ResetOffset();
	msg.m_offset = msg.GetHeaderSize();

	if (conn == nullptr)
	{
		return;
	}

	uint8_t playerIndex;
	msg.Read<uint8_t>(&playerIndex);

	Vector2 newPos = Vector2::ZERO;
	eGame_PlayerDirection newDir = eGame_PlayerDirection_NORTH;
	uint16_t score = 0;


	msg.Read<Vector2>(&newPos);
	msg.Read<uint8_t>(&newDir);
	msg.Read<uint16_t>(&score);

	Player* connPlayer = g_theGame->m_clientWorld->FindPlayerByIndex(playerIndex);

	//	ASSERT_OR_DIE(connPlayer != nullptr, "Error: Have a connection with no attached player pointer");
	if (connPlayer == nullptr)
	{
		return;
	}

	if (connPlayer->m_score < score);
	{
		connPlayer->m_score = score;
	}

	if (m_clientWorld->m_myPlayer != nullptr)
	{
		if (m_clientWorld->m_myPlayer->m_ownerNetIdx == connPlayer->m_ownerNetIdx)
		{
			return;
		}
	}

	
	

	

	connPlayer->m_direction = newDir;
	connPlayer->m_desiredPos = newPos;
	//connPlayer->MoveAndUpdateBounds(newPos);
}

void TheGame::SendAllConnectionsCurrentNetClock()
{
	if (!g_netSession->m_isHost)
		return;
	NetMessage netTickUpdate(eMessageID_NET_GAME_NET_TIME_UPDATE);

	netTickUpdate.m_definition = g_netSession->FindDefinition(netTickUpdate.m_messageType);
	netTickUpdate.WriteToMessage<double>(&m_hostWorld->m_actualTime);

	netTickUpdate.FinalizeMessage();
	g_netSession->SendMessageToAllConnections(netTickUpdate);
}

void DrawDebugArrow(Command& args)
{
    
    if (args.m_argList.empty())
    {
        g_theRenderer->AddDebugArrow(Vector3(1.f, 1.f, 1.f), Vector3(10.f, 50.f, 10.f), Rgba::YELLOW, 2.f, RenderCommand::XRAY);
    }
}

void TestJobFunction(Job* job)
{
	job;
}


void OnUpdateClientToHostReceive(const NetSender &from, NetMessage &msg)
{
	if (g_netSession->m_isHost)
	{
		g_theGame->OnUpdateReceiveHost(from, msg);
	}
}
void OnUpdateHostToClientReceive(const NetSender &from, NetMessage &msg)
{
	//Update other player's positions from their update message


	

	if (g_theGame->m_clientWorld->m_myPlayer != nullptr)
	{
		g_theGame->OnUpdateReceiveClient(from, msg);
	}
	/*
	NetConnection* conn = from.connection;

	if (conn == nullptr)
	{
		return;
	}

	Player* connPlayer = g_theGame->m_clientWorld->FindPlayerByIndex(conn->m_index);

//	ASSERT_OR_DIE(connPlayer != nullptr, "Error: Have a connection with no attached player pointer");
	if (connPlayer == nullptr)
	{
		return;
	}

	msg.ResetOffset();
	msg.m_offset = msg.GetHeaderSize();
	Vector2 newPos = Vector2::ZERO;
	eGame_PlayerDirection newDir = eGame_PlayerDirection_NORTH;

	msg.Read<Vector2>(&newPos);
	msg.Read<uint8_t>(&newDir);

	connPlayer->m_direction = newDir;
	connPlayer->MoveAndUpdateBounds(newPos);
	*/

	/*
	uint8_t index = from.connection.get_index() //constant for the lifetime of the connection
		FindObjectByIndex(idx) //find object that owns this indexvec2 pos;
		Msg->read(&pos);
	Obj->pos = pos;
	*/
}

void OnTestReliableReceive(const NetSender &from, NetMessage &msg)
{

	std::string connStr =  "Received reliable from connection " + std::to_string(from.connection->m_index) + " [" + std::to_string(msg.m_reliableID) + "]";
	g_theConsole->ConsolePrint(connStr, Rgba::LTBLUE);
}

void OnInOrderReliableReceive(const NetSender &from, NetMessage &msg)
{

	std::string connStr = "Received sequence from connection " + std::to_string(from.connection->m_index) + " [" + std::to_string(msg.m_sequenceID) + "]";
	g_theConsole->ConsolePrint(connStr, Rgba::LTBLUE);
}

void OnPlayerCreateRequest( const NetSender &from,  NetMessage &request)
{

	NetPlayerData incomingData = NetPlayerData();
	incomingData.ReadPlayerDataFromMessage(&request);
	NetPlayerData* currentlyExistingPlayerData = g_theGame->m_hostWorld->FindPlayerDataByConnectionIndex(from.connection->m_index);//g_theGame->FindPlayerDataByConnectionIndex(from.connection->m_index);


	NetPlayerData* newPlayer = nullptr;
	if (currentlyExistingPlayerData != nullptr && from.connection != g_netSession->m_myConnection)
	{
		//Deny

		NetMessage deny(eMessageID_NET_GAME_PLAYER_DENY);
		deny.m_definition = g_netSession->FindDefinition(deny.m_messageType);
		
		deny.WriteToMessage<uint32_t>(&incomingData.m_creationNuonce);
		deny.FinalizeMessage();
		g_netSession->SendMessageDirect(from.address, deny);
		return;

	}
	else if (currentlyExistingPlayerData != nullptr && from.connection == g_netSession->m_myConnection)
	{
		newPlayer = currentlyExistingPlayerData;
	}
	else
	{
		newPlayer = g_theGame->m_hostWorld->CreatePlayerData();
		//g_theGame->m_netPlayerDatasHost.push_back(newPlayer);
	}

	newPlayer->m_creationNuonce = incomingData.m_creationNuonce;
	newPlayer->m_ownerConnectionIndex = incomingData.m_ownerConnectionIndex;
	newPlayer->m_playerIndex = incomingData.m_ownerConnectionIndex;

	//Send to new player all current player data
	for each (NetPlayerData* data in g_theGame->m_hostWorld->m_netPlayerDatas)
	{
		NetMessage* existingPlayerMsg = new NetMessage(eMessageID_NET_GAME_PLAYER_JOINED);
		existingPlayerMsg->m_definition = g_netSession->FindDefinition(existingPlayerMsg->m_messageType);
		data->WritePlayerDataToMessage(existingPlayerMsg);
		//create.WriteToMessage<uint32_t>(&incomingData.m_creationNuonce); // really only meant for creator, but whatever...
		existingPlayerMsg->FinalizeMessage();
		from.connection->AddMessage(existingPlayerMsg);
	}

	
	
	auto netObjIter = g_theGame->m_hostWorld->m_netObjects.begin();
	while(netObjIter != g_theGame->m_hostWorld->m_netObjects.end())
	{
		NetObject* thisObj = netObjIter->second;

		NetMessage create(eMessageID_NET_GAME_OBJECT_CREATE);

		create.m_definition = g_netSession->FindDefinition(create.m_messageType);
		create.WriteToMessage<uint16_t>(&thisObj->net_id);
		
		create.WriteToMessage<uint32_t>(&thisObj->type_id);
		create.WriteToMessage<uint8_t>(&thisObj->owning_player_index);
		create.WriteToMessage<uint8_t>(&thisObj->owning_connection_index);
		thisObj->type->SendCreate(thisObj, &create);
		create.FinalizeMessage();
		g_netSession->SendMessageToAllConnections(create);
		++netObjIter;
	}
	
	

	g_theGame->m_hostWorld->AddPlayerAtIndex(incomingData.m_ownerConnectionIndex);//May need to add callbacks here
	g_theConsole->ConsolePrint("Host adding player at index: " + std::to_string(incomingData.m_ownerConnectionIndex));
	

	NetMessage create(eMessageID_NET_GAME_PLAYER_JOINED);
	create.m_definition = g_netSession->FindDefinition(create.m_messageType);
	newPlayer->WritePlayerDataToMessage(&create);
	//create.WriteToMessage<uint32_t>(&incomingData.m_creationNuonce); // really only meant for creator, but whatever...
	create.FinalizeMessage();
	g_netSession->SendMessageToAllConnections(create);


	/*
   // Okay, it's going to succeed.


  
   // Host version - finds a free index, and assigns it to the player
   // and calls the PlayerJoined callbacks if available.
   player->player_index = AssignIndexToPlayer(player); // however you want to manage this

   NetMessage create( NET_GAME_NEW_PLAYER );
   create.write<uint32_t>( nuonce ); // really only meant for creator, but whatever...
   player->write_to_packer( create );

   from.session->send_to_all( create );
}

	*/
}

void OnPlayerJoin(const NetSender &from, NetMessage &msg)
{

	if (g_netSession->m_isHost)
	{
		g_theGame->m_hostWorld->OnPlayerJoin(from, msg);
	}


	g_theGame->m_clientWorld->OnPlayerJoin(from, msg);

	/*
	NetPlayerData incomingPlayerData;

	incomingPlayerData.ReadPlayerDataFromMessage(&msg);

	NetPlayerData* newPlayer = nullptr;

	if (incomingPlayerData.m_ownerConnectionIndex == from.session->m_myConnection->m_index)
	{
		//Get MY player data via Nounce
		newPlayer = g_theGame->FindMyPlayerDataByNounce(incomingPlayerData.m_creationNuonce);
		//newPlayer = g_theGame->FindPlayerDataByConnectionIndex(incomingPlayerData.m_ownerConnectionIndex);
		

		if (newPlayer == nullptr)
		{
			return;
		}
	}
	else
	{
		//Make a New player for the other guy
		newPlayer = g_theGame->FindMyPlayerDataByNounce(incomingPlayerData.m_creationNuonce);

		if (newPlayer != nullptr)
		{
			return;
		}
		newPlayer = g_theGame->CreatePlayerData();
		
	}


	//copy the read in data to this player

	newPlayer->m_creationNuonce = incomingPlayerData.m_creationNuonce;
	newPlayer->m_ownerConnectionIndex = incomingPlayerData.m_ownerConnectionIndex;
	newPlayer->m_playerIndex = incomingPlayerData.m_ownerConnectionIndex;
	//AssignPlayerToIndex
	Player* player = g_theGame->m_clientWorld->AddPlayerAtIndex(newPlayer->m_ownerConnectionIndex);
	if (player != nullptr)
	{
		std::string joinStr = "Player Joined At: " + std::to_string(newPlayer->m_ownerConnectionIndex);
		g_theConsole->ConsolePrint(joinStr);
	}
	*/
}

void OnPlayerDeny(const NetSender &from, NetMessage &msg)
{
	g_theConsole->ConsolePrint("Player Denied", Rgba::RED);
}

void OnNetObjCreate(const NetSender &from, NetMessage &msg)
{
	if (g_netSession->m_isHost)
	{
		//g_theGame->m_hostWorld->OnProcessObjectCreate(from, msg);
	}
	msg.ResetOffset();

	if (g_theGame->m_clientWorld->m_myPlayer != nullptr)
	{
		g_theGame->m_clientWorld->OnProcessObjectCreate(from, msg);
	}
	g_theConsole->ConsolePrint("Obj Created", Rgba::RED);
}

void OnNetObjDestroy(const NetSender &from, NetMessage &msg)
{

	if (g_netSession->m_isHost)
	{
		g_theGame->m_hostWorld->OnProcessObjectDestroy(from, msg);
	}
	msg.ResetOffset();

	if (g_theGame->m_clientWorld->m_myPlayer != nullptr)
	{
		g_theGame->m_clientWorld->OnProcessObjectDestroy(from, msg);
	}
	g_theConsole->ConsolePrint("Obj Destroy", Rgba::RED);
}

void OnNetObjUpdate(const NetSender &from, NetMessage &msg)
{
	if (g_netSession->m_isHost)
	{
		g_theGame->m_hostWorld->OnProcessObjectUpdateHost(from, msg);
	}
	msg.ResetOffset();

	if (g_theGame->m_clientWorld->m_myPlayer != nullptr)
	{
		g_theGame->m_clientWorld->OnProcessObjectUpdateClient(from, msg);
	}
}

void OnUpdateNetTime(const NetSender &from, NetMessage &msg)
{
	if (g_theGame->m_clientWorld->m_myPlayer != nullptr)
	{
		g_theGame->m_clientWorld->OnUpdateNetTime(from, msg);
	}
}

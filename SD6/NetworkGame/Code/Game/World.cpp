#include "Game/World.hpp"
#include "Game/TheApp.hpp"
//#include "Game/GameCommon.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/TheGame.hpp"
#include <string>
#include "Engine/Core/Time.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Entity.hpp"
#include "Game/EnemyShip.hpp"
#include "Game/Meteor.hpp"
#include "Game/Star.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Core/Console.hpp"
#include "Engine/Renderer/SpriteRenderer/Sprite.hpp"
#include "Engine/Renderer/SpriteRenderer/AnimatedSprite.hpp"
#include "Game/Map.hpp"
#include "Engine/Core/EngineCommon.hpp"

#include "Game/Coin.hpp"

#include "Game/Net/NetGameCommon.hpp"
#include "Engine/Net/Session/NetSession.hpp"
#include "Engine/Net/Session/NetConnection.hpp"
#include "Game/Net/NetObjectTypesSystem.hpp"
#include "Game/Net/NetPlayerData.hpp"
#include "Game/Net/NetObject.hpp"

const int WORLD_X = 4;
const int WORLD_Y = 4;
const int STAR_FRAME_FREQUENCY = 2;
const float MAP_ROW_SPAWN_RATE = 0.1f;
const float WAVE_TITLE_SHOW_TIME = 2.f;


World::World(bool isHostWorld)
	: m_startTrace(0.f, 0.f, 0.f)
	, m_endTrace(0.f, 0.f, 0.f)
	, m_myPlayer (nullptr)
	, m_starFrameCount(0)
	, m_worldTimer(0)
	, m_mapTick(0)
	, m_mapIndex(0)
	, m_rowIndex(0)
	, m_waveSpriteTick(0)
	, m_currentWaveSprite(nullptr)
	, m_isHostWorld(isHostWorld)
	, m_currentNetworkObjectID(0xff)
	, m_isWorldRunning(false)
	, m_actualTime(0)
	, m_prevActualTime(0)
	, m_desiredTime(0)
	, m_prevDesiredTime(0)
{

	//m_player = Player(Vector3(2.3f, 2.3f, 69.f + (1.85f / 2.f)));
	
	//m_camera.m_position.SetXYZ(2.f, 2.f, 70.f);
	//m_camera.m_position = m_player->m_pos;
	m_moveMod = 1.f;
	for (int i = 0; i < 1; i++)
	{
		//float randX = GetRandomFloatBetween(g_spriteRenderSystem->GetVirtualBounds().m_mins.x, g_spriteRenderSystem->GetVirtualBounds().m_maxs.x);
		//m_entities.push_back(new EnemyShip(Vector2(randX, g_spriteRenderSystem->GetVirtualBounds().m_maxs.y)));

	}

	m_mapList.push_back(new Map("Data/Maps/Map1.png"));
	m_mapList.push_back(new Map("Data/Maps/Map2.png"));

// 	m_spriteAttractScreen = Sprite::Create("attract", eGame_SpriteLayers::LAYER_ATTRACT);
// 	m_spriteAttractScreen->m_scale = Vector2(0.2f, 0.2f);

	m_spriteGameOverScreen = Sprite::Create("gameover", eGame_SpriteLayers::LAYER_UI);
	m_spriteGameOverScreen->m_scale = Vector2(0.5f, 0.5f);
	m_spriteGameOverScreen->SetEnabled(false);

	m_spriteVictoryScreen = Sprite::Create("victory", eGame_SpriteLayers::LAYER_UI);
	m_spriteVictoryScreen->m_scale = Vector2(0.5f, 0.5f);
	m_spriteVictoryScreen->SetEnabled(false);


	m_spriteStrikerRound = Sprite::Create("strikerRound", eGame_SpriteLayers::LAYER_UI);
	m_spriteStrikerRound->m_scale = Vector2(0.2f, 0.2f);
	m_spriteStrikerRound->m_position.y += 1.f;
	m_spriteStrikerRound->SetEnabled(false);



	m_spriteSpeedRound = Sprite::Create("speedRound", eGame_SpriteLayers::LAYER_UI);
	m_spriteSpeedRound->m_scale = Vector2(0.2f, 0.2f);
	m_spriteSpeedRound->m_position.y += 1.f;
	m_spriteSpeedRound->SetEnabled(false);

	m_spriteSlayerRound = Sprite::Create("slayerRound", eGame_SpriteLayers::LAYER_UI);
	m_spriteSlayerRound->m_scale = Vector2(0.2f, 0.2f);
	m_spriteSlayerRound->m_position.y += 1.f;
	m_spriteSlayerRound->SetEnabled(false);





	//attractAnim
	m_attractAnimScreen = AnimatedSprite::Create("attractAnim", eGame_SpriteLayers::LAYER_ATTRACT);
	m_attractAnimScreen->m_scale = Vector2(0.2f, 0.2f);
	m_attractAnimScreen->m_position.y += 1.7f;
	m_attractAnimScreen->m_isPlaying = !isHostWorld;


	m_attractReadyAnimScreen = AnimatedSprite::Create("startAnim", eGame_SpriteLayers::LAYER_ATTRACT);
	m_attractReadyAnimScreen->m_scale = Vector2(0.1f, 0.1f);
	m_attractReadyAnimScreen->m_position.y -= 3.2f;
	m_attractReadyAnimScreen->m_isPlaying = false;


	m_background = Sprite::Create("coin_background", 0);
	m_background->m_scale = Vector2(2.75f,2.f);
}

World::~World()
{

	for each (Entity* ent in m_entities)
	{
		delete ent;
	}

	for each (Entity* ent in m_entitiesBackGround)
	{
		delete ent;
	}
	for each (Entity* ent in m_entitiesToAdd)
	{
		delete ent;
	}
	for each (Entity* ent in m_entitiesBackGroundToAdd)
	{
		delete ent;
	}

	for each (Player* player in m_players)
	{
		if (player == m_myPlayer)
		{
			m_myPlayer = nullptr;
		}
		delete player;
	}

	for each (Map* map in m_mapList)
	{
		delete map;
	}
	
	for each (Coin* coin in m_coinList)
	{
		delete coin;
	}

	auto netPlayerDataIter = m_netPlayerDatas.begin();
	while (netPlayerDataIter != m_netPlayerDatas.end())
	{
		delete *netPlayerDataIter;
		netPlayerDataIter = m_netPlayerDatas.erase(netPlayerDataIter);
	}

	auto netObjIter = m_netObjects.begin();
	while (netObjIter != m_netObjects.end())
	{
		delete netObjIter->second;
		netObjIter = m_netObjects.erase(netObjIter);
	}


	//delete m_spriteAttractScreen;
	delete m_spriteGameOverScreen;
	delete m_spriteVictoryScreen;
	delete m_spriteStrikerRound;
	delete m_spriteSpeedRound;
	delete m_spriteSlayerRound;

	delete m_currentWaveSprite;

	delete m_attractAnimScreen;
	delete m_attractReadyAnimScreen;
	delete m_background;
}

Player* World::FindPlayerByIndex(uint8_t index)
{
	Player* outPlayer = nullptr;
	for (auto playerIter = m_players.begin(); playerIter != m_players.end(); ++playerIter)
	{
		if ((*playerIter)->m_ownerNetIdx == index)
		{
			outPlayer = *playerIter;
			break;
		}
	}

	return outPlayer;

}

Player* World::AddPlayerAtIndex(uint8_t index)
{

	if (FindPlayerByIndex(index) != nullptr)
	{
		return nullptr;
	}

	Player* newPlayer = new Player(Vector2(1.f, 0.f));
	newPlayer->m_ownerNetIdx = index;
	newPlayer->SetColorFromPlayerIndex(index);
	//#TODO hide player if host
	if (m_isHostWorld)
	{
		newPlayer->m_sprite->SetEnabled(false);
		//newPlayer->m_sprite->m_color = Rgba::GREEN;
	}

	m_players.push_back(newPlayer);

	if (index == g_netSession->m_myConnection->m_index)
	{
		m_myPlayer = newPlayer;
		m_myPlayer->m_ownerNetIdx = index;
	}

	return newPlayer;
}

void World::RemovePlayerAtIndex(uint8_t index)
{
	Player* deletePlayer = nullptr;
	for (auto playerIter = m_players.begin(); playerIter != m_players.end(); ++playerIter)
	{
		if ((*playerIter)->m_ownerNetIdx == index)
		{
			deletePlayer = *playerIter;
			m_players.erase(playerIter);
			break;
		}
	}


	if (deletePlayer != nullptr)
	{
		if (m_myPlayer == deletePlayer)
		{
			m_myPlayer = nullptr;
		}
		RemoveAllNetObjectsOwnedByPlayerIndex(index);
		RemovePlayerDataAtIndex(index);

		delete deletePlayer;
		deletePlayer = nullptr;
	}
}

void World::Update(float deltaSeconds)
{
	g_spriteRenderSystem->EnableLayer(eGame_SpriteLayers::LAYER_BACKGROUND);
	if (!m_isHostWorld)
	{
		if (m_coinList.size() > 0 && m_coinList[0]->m_isDead)
		{
			int breakHere = 0;
		}
	}

	UpdateHostTime(deltaSeconds);
	UpdateClientTime(deltaSeconds);


	float actualDT = GetActualNetDeltaSeconds();

	if (m_isHostWorld)
	{
		if (m_netObjsToErase.size() > 0)
		{
			for each (uint16_t netID in m_netObjsToErase)
			{
				RemoveNetObjAndSignalOthers(netID);
			}
		}
		m_netObjsToErase.clear();
	}

	if (m_myPlayer != nullptr)
	{
		UpdateCameraAndPlayer(actualDT);
	}

	for each (Player* thisPlayer in m_players)
	{
		if (thisPlayer == m_myPlayer)
			continue;
		thisPlayer->LerpToDesiredPos();
		thisPlayer->PredictMovement();
	}

	for each (Coin* coin in m_coinList)
	{
		coin->Update(actualDT);
		if (m_isHostWorld)
		{
			for each (Player* thisPlayer in m_players)
			{
				if (coin->HandleCollisionWithPlayer(thisPlayer))
				{
					//We collided. Trigger collision events
					thisPlayer->m_score++;
					m_netObjsToErase.push_back(coin->m_netObj->net_id);
					//RemoveNetObjAndSignalOthers(coin->m_netObj->net_id);
				}
			}
			
		}
	}

	


	//All the other updates

	/*
	if (g_theGame->m_state.GetState() == STATE_READY)
	{
		m_attractAnimScreen->m_animClock->m_scale = 3.f;
		m_attractReadyAnimScreen->m_isPlaying = true;
		m_worldTimer += deltaSeconds;
		
	}
	else
	{
		m_attractAnimScreen->m_animClock->m_scale = 0.8f;
	}

	m_attractAnimScreen->Update();
	m_attractReadyAnimScreen->Update();

	//MakeStars();
	UpdateBackGroundEntities(deltaSeconds);

	if (g_theGame->m_state.GetState() == STATE_PLAYING)
	{ 
		LoadMapEnts(deltaSeconds);

		if (g_theGame->m_state.GetState() != STATE_PLAYING)
		{
			return;
		}
		UpdateCameraAndPlayer(deltaSeconds);
		UpdateEntities(deltaSeconds);


		if (m_player->m_isDead)
		{
			g_theGame->m_state.SetState(STATE_GAMEOVER);
			g_spriteRenderSystem->DisableAllButThisLayer(eGame_SpriteLayers::LAYER_UI);
			g_spriteRenderSystem->EnableLayer(eGame_SpriteLayers::LAYER_UI);
			g_spriteRenderSystem->EnableLayer(eGame_SpriteLayers::LAYER_BACKGROUND);
			m_spriteGameOverScreen->SetEnabled(true);
		}
	}
	*/
	
}

void World::UpdateHostTime(float deltaSeconds)
{
	if (!m_isHostWorld)
		return;
	

	m_prevActualTime = m_actualTime;
	m_actualTime += (double)deltaSeconds;
}

void World::UpdateClientTime(float deltaSeconds)
{
	if (m_isHostWorld)
		return;

	//#TODO Figure out how to smoothly move to Desired by speeding up or slowing down deltaSeconds
	float moddedDelta = deltaSeconds;
	m_prevActualTime = m_actualTime;
	m_prevDesiredTime = m_desiredTime;

	if (IsActualTimeWithinDesiredThreshold())
	{
		moddedDelta = deltaSeconds;
	}
	else
	{
		double offset = abs(m_actualTime - m_desiredTime);
		if (m_desiredTime < m_actualTime)
		{
			//We are ahead of NetTime. Slow down

			offset = ClampFloat(offset, 0.f, GAME_NET_TIME_UPDATE_TICKRATE);
			double dialationAmount = RangeMap(offset, 0.f, GAME_NET_TIME_UPDATE_TICKRATE, 1.f, 0.5f);
			moddedDelta = deltaSeconds * dialationAmount;
		}
		else
		{
			//We are behind NetTime. Speed up.
			offset = ClampFloat(offset, 0.f, GAME_NET_TIME_UPDATE_TICKRATE);
			double dialationAmount = RangeMap(offset, 0.f, GAME_NET_TIME_UPDATE_TICKRATE, 1.f, 1.5f);
			moddedDelta = deltaSeconds * dialationAmount;
		}
	}

	m_actualTime += moddedDelta;
	m_desiredTime += deltaSeconds;
	//Shift both desired and actual
	//if there is a difference between desired and actual (desired is what we get over the net)
	//Speed up or slow down delta seconds then apply it to actual
}

float World::GetActualNetDeltaSeconds()
{
	return ((float)m_actualTime - (float)m_prevActualTime);
}

void World::StartRunningWorld(double netTime)
{
	m_isWorldRunning = true;
	m_actualTime = netTime;
	m_prevActualTime = netTime;

	m_desiredTime = netTime;
	m_prevDesiredTime = netTime;
}



void World::OnUpdateNetTime(const NetSender &from, NetMessage &msg)
{
	msg.ResetOffset();
	double netTimeStamp = 0;
	msg.ReadFromMessage<double>(&netTimeStamp);
	double halfRTT = from.connection->m_rtt / 2.f;
	netTimeStamp = netTimeStamp + halfRTT;
	m_desiredTime = (0.8f * m_desiredTime) + (netTimeStamp * 0.2f);
}

bool World::IsActualTimeWithinDesiredThreshold()
{
	if (abs(m_actualTime - m_desiredTime) < DESIRED_TIME_THRESHOLD)
	{
		return true;
	}
	return false;
}

void World::Render()
{
// 	g_theRenderer->SetAlphaTest(true, 0.5f);
// 	//g_theRenderer->DrawAABB3Wireframe(m_player->m_bounds, Rgba::WHITE);
// 	g_theRenderer->SetAlphaTest(false);

	if (m_isHostWorld)
		return;

	static BitmapFontMeta* bitmapFont = BitmapFontMeta::CreateOrGetFont("Data/Fonts/ArialFont.fnt");
	float offsetY = g_theApp->WINDOW_PHYSICAL_HEIGHT - 80.f;
	int lineIndex = 0;
	for each (Player* thisPlayer in m_players)
	{
		
		std::string finalPlayerString = "Player (" + std::to_string(thisPlayer->m_ownerNetIdx) + "): " + std::to_string(thisPlayer->m_score);
		g_theRenderer->DrawTextMeta2D(Vector3(20.f, offsetY - (lineIndex * 20.f), 0.f), 0.4f,
			finalPlayerString,
			Rgba::WHITE, bitmapFont);
		lineIndex++;
	}

}

bool World::IsHostWorld()
{
	return m_isHostWorld;
}

bool World::IsValidClientWorld()
{
	if (!m_isHostWorld && m_myPlayer != nullptr)
	{
		return true;
	}
	return false;
}

NetPlayerData* World::CreatePlayerData()
{
	NetPlayerData* newPlayer = new NetPlayerData();

	m_netPlayerDatas.push_back(newPlayer);

	return newPlayer;
}

void World::RequestMyPlayer(NetConnection* myConnection)
{

	NetPlayerData* myPlayerData = CreatePlayerData();

	//Player Index is Invalid for now.
	myPlayerData->m_ownerConnectionIndex = myConnection->m_index;
	myPlayerData->m_creationNuonce = GetNuonce();

	//Game Specific Data goes here

	//NetMessage PlayerRequest
	NetMessage* playerRequestMessage = new NetMessage(eMessageID_NET_GAME_REQUEST_PLAYER);
	playerRequestMessage->m_definition = g_netSession->FindDefinition(playerRequestMessage->m_messageType);
	playerRequestMessage->AssembleMessage();

	myPlayerData->WritePlayerDataToMessage(playerRequestMessage);
	playerRequestMessage->FinalizeMessage();
	g_netSession->GetConnection(0)->AddMessage(playerRequestMessage);
}

NetPlayerData* World::FindPlayerDataByConnectionIndex(uint8_t connIndex)
{
	for each (NetPlayerData* data in m_netPlayerDatas)
	{
		if (data->m_ownerConnectionIndex == connIndex)
		{
			return data;
		}
	}
	return nullptr;
}

NetPlayerData* World::FindMyPlayerDataByNounce(uint32_t nounce)
{
	for each (NetPlayerData* data in m_netPlayerDatas)
	{
		if (data->m_creationNuonce == nounce)
		{
			return data;
		}
	}
	return nullptr;
}

void World::RemovePlayerDataAtIndex(uint8_t connIndex)
{
	auto netPlayerDataIter = m_netPlayerDatas.begin();
	while (netPlayerDataIter != m_netPlayerDatas.end())
	{
		NetPlayerData* data = *netPlayerDataIter;
		if (data->m_ownerConnectionIndex == connIndex)
		{
			delete *netPlayerDataIter;
			netPlayerDataIter = m_netPlayerDatas.erase(netPlayerDataIter);
			break;
		}
		else
		{
			++netPlayerDataIter;
		}
	}
}

void World::RemoveAllNetObjectsOwnedByPlayerIndex(uint8_t playerIndex)
{
	auto netObjIter = m_netObjects.begin();
	while (netObjIter != m_netObjects.end())
	{
		NetObject* thisObj = netObjIter->second;
		if (thisObj->owning_player_index = playerIndex)
		{
			delete thisObj;
			netObjIter = m_netObjects.erase(netObjIter);
		}
		else
		{
			++netObjIter;
		}
	}
	
}

uint16_t World::NetGenerateNetworkID()
{
	m_currentNetworkObjectID++;
	return m_currentNetworkObjectID;
}

void World::UpdateCameraAndPlayer(float deltaSeconds)
{
	if (!g_theApp->m_inputSystem.m_isInFocus)
		return;
	//Raycast endpos : dist * ForwardXYZ().Norm + cameraPos
	IntVector2 cursorPos = g_theApp->m_inputSystem.GetMousePos();
	g_theApp->m_inputSystem.ShowMouse(false);
	g_theApp->m_inputSystem.SetMousePos(snapBackPos.x, snapBackPos.y);
	IntVector2 cursorDelta = cursorPos - snapBackPos;


	if (cursorDelta.x != 0 || cursorDelta.y != 0)
	{
		//m_camera.m_orientation.m_yawDegreesAboutZ -= mouseSensitivity  * (float)cursorDelta.x;
		//m_camera.m_orientation.m_pitchDegreesAboutY += mouseSensitivity  * (float)cursorDelta.y;
		g_camera.m_orientation.m_yawDegreesAboutZ += mouseSensitivity  * (float)cursorDelta.x;
		g_camera.m_orientation.m_pitchDegreesAboutX -= mouseSensitivity  * (float)cursorDelta.y;
	}

	//m_camera.FixAndClampAngles();
	g_camera.FixAndClampAngles();




	m_myPlayer->ClearMoveStates();

	



	if (!g_theConsole->m_isOpen)
	{


		//Controller Input
		Vector2 leftStick = g_theApp->m_controllers[0].GetLeftStickF();
		Vector2 rightStick = g_theApp->m_controllers[0].GetRightStickF();

		if (leftStick.x != 0 || leftStick.y != 0 || rightStick.x != 0 || rightStick.y != 0)
		{
			float right = leftStick.x;
			float forward = leftStick.y;

			//TODO set mod
			m_myPlayer->m_moveModUp = forward;
			m_myPlayer->m_moveModRight = right;
			//Forward
			if (forward > 0)
			{
				m_myPlayer->m_isMovingForward = true;
				m_myPlayer->m_isMovingBackwards = false;

			}

			else if (forward < 0)
			{
				m_myPlayer->m_isMovingForward = false;
				m_myPlayer->m_isMovingBackwards = true;
			}
			else
			{
				m_myPlayer->m_isMovingForward = false;
				m_myPlayer->m_isMovingBackwards = false;
			}


			if (right > 0)
			{
				m_myPlayer->m_isMovingRight = true;
				m_myPlayer->m_isMovingLeft = false;
			}
			else if (right < 0)
			{
				m_myPlayer->m_isMovingRight = false;
				m_myPlayer->m_isMovingLeft = true;
			}
			else
			{
				m_myPlayer->m_isMovingRight = false;
				m_myPlayer->m_isMovingLeft = false;
			}

		}
		//Keyboard input
		else
		{
			m_myPlayer->m_moveModUp = 1.f;
			m_myPlayer->m_moveModRight = 1.f;
			if (g_theApp->m_inputSystem.IsKeyDown('W') || g_theApp->m_inputSystem.IsKeyDown(VK_UP))
			{
				m_myPlayer->m_isMovingForward = true;
				m_myPlayer->m_isMovingBackwards = false;
			}
			else if (g_theApp->m_inputSystem.IsKeyDown('S') || g_theApp->m_inputSystem.IsKeyDown(VK_DOWN))
			{
				m_myPlayer->m_moveModUp = -1.f;
				m_myPlayer->m_isMovingForward = false;
				m_myPlayer->m_isMovingBackwards = true;
			}
			if (g_theApp->m_inputSystem.IsKeyDown('A') || g_theApp->m_inputSystem.IsKeyDown(VK_LEFT))
			{
				m_myPlayer->m_moveModRight = -1.f;
				m_myPlayer->m_isMovingLeft = true;
				m_myPlayer->m_isMovingRight = false;
			}
			else if (g_theApp->m_inputSystem.IsKeyDown('D') || g_theApp->m_inputSystem.IsKeyDown(VK_RIGHT))
			{
				m_myPlayer->m_isMovingLeft = false;
				m_myPlayer->m_isMovingRight = true;
			}
		}



		if (g_theApp->m_inputSystem.IsKeyDown(VK_SPACE) || g_theApp->m_controllers[0].IsButtonDown(XBOX_A))
		{
			m_myPlayer->m_isShooting = true;
		}
		else
		{
			m_myPlayer->m_isShooting = false;
		}
	}
	

	m_myPlayer->Update(deltaSeconds);




	//TODO: MOUSE WHEEL
	int mouseWheelDir = g_theApp->m_inputSystem.GetMouseWheelDirection();

	if (mouseWheelDir != 0)
	{
		/*
		SoundID sound = g_audio->CreateOrGetSound(BLOCK_SELECT_SOUND);
		g_audio->PlaySound(sound, 0.5f);

		if (mouseWheelDir > 0)
		{
			m_selectedBlockIndex++;
			if (m_selectedBlockIndex > 9)
				m_selectedBlockIndex = 0;
		}
		else
		{
			m_selectedBlockIndex--;
			if (m_selectedBlockIndex < 0)
				m_selectedBlockIndex = 9;
		}
		*/
	}




	//Vector3 cameraForwardDirection = m_camera.GetForwardXYZ().GetNormalized();
	Vector3 cameraForwardDirection = g_camera.GetForwardXYZ().GetNormalized();
	//DebuggerPrintf("Camera Pos: X: %f,Y: %f, Z:%f\nCamForwardXYZ: X: %f,Y: %f, Z:%f\n", g_camera.m_position.x, g_camera.m_position.y, g_camera.m_position.z, cameraForwardDirection.x, cameraForwardDirection.y, cameraForwardDirection.z);

	


}

void World::UpdateEntities(float deltaSeconds)
{
	for each (Entity* ent in m_entities)
	{
		if (m_isHostWorld)
		{
			ent->m_sprite->SetEnabled(false);
		}
		ent->Update(deltaSeconds);
	}


	std::vector<Entity*>::iterator addIter = m_entitiesToAdd.begin();
	while (addIter != m_entitiesToAdd.end()) {
		Entity* entToAdd = *addIter;
		if (m_isHostWorld)
		{
			entToAdd->m_sprite->SetEnabled(false);
		}
		m_entities.push_back(entToAdd);
		addIter = m_entitiesToAdd.erase(addIter);
	}


	//Clean dead ents
	std::vector<Entity*>::iterator removeIter = m_entities.begin();
	while (removeIter != m_entities.end()) {
		Entity* b = *removeIter;
		if (b->m_isDead)
		{
			removeIter = m_entities.erase(removeIter);
			delete b;
		}
		else
			++removeIter;
	}
}

void World::UpdateBackGroundEntities(float deltaSeconds)
{
	for each (Entity* ent in m_entitiesBackGround)
	{
		if (m_isHostWorld)
		{
			ent->m_sprite->SetEnabled(false);
		}
		ent->Update(deltaSeconds);
	}


	std::vector<Entity*>::iterator addIter = m_entitiesBackGroundToAdd.begin();
	while (addIter != m_entitiesBackGroundToAdd.end()) {
		Entity* entToAdd = *addIter;
		if (m_isHostWorld)
		{
			entToAdd->m_sprite->SetEnabled(false);
		}
		m_entitiesBackGround.push_back(entToAdd);
		addIter = m_entitiesBackGroundToAdd.erase(addIter);
	}


	//Clean dead ents
	std::vector<Entity*>::iterator removeIter = m_entitiesBackGround.begin();
	while (removeIter != m_entitiesBackGround.end()) {
		Entity* b = *removeIter;
		if (b->m_isDead)
		{
			removeIter = m_entitiesBackGround.erase(removeIter);
			delete b;
		}
		else
			++removeIter;
	}
}

void World::LoadMapEnts(float deltaSeconds)
{
	m_mapTick += deltaSeconds;

	if (m_currentWaveSprite != nullptr)
	{
		m_waveSpriteTick += deltaSeconds;
		if (m_waveSpriteTick > WAVE_TITLE_SHOW_TIME)
		{
			m_currentWaveSprite->SetEnabled(false);
			m_currentWaveSprite = nullptr;
		}
	}

	if (m_mapTick > MAP_ROW_SPAWN_RATE)
	{
		m_mapTick = 0;
		int rowCount = m_mapList[m_mapIndex]->m_rowData.size() - 1;
		
		if (m_rowIndex > rowCount)
		{
			//Map is done. Load the next one.
			m_mapIndex++;
			m_rowIndex = 0;
			//Any more maps? if not, player wins.
			int mapCount = m_mapList.size() - 1;
			if (m_mapIndex > mapCount)
			{
				g_theGame->m_state.SetState(STATE_VICTORY);
				m_spriteVictoryScreen->SetEnabled(true);
				g_spriteRenderSystem->DisableAllButThisLayer(LAYER_UI);
				g_spriteRenderSystem->EnableLayer(LAYER_BACKGROUND);
				return;
			}
			else
			{
				EnableWaveSpriteForCurrentMap();
				SpawnRow(m_mapList[m_mapIndex]->m_rowData[m_rowIndex]);
				m_rowIndex++;
			}
		}
		else
		{
			SpawnRow(m_mapList[m_mapIndex]->m_rowData[m_rowIndex]);
			m_rowIndex++;
		}

	}
}

void World::EnableWaveSpriteForCurrentMap()
{
	Map* currentMap = m_mapList[m_mapIndex];

	switch (currentMap->m_mapType)
	{
	case MAPTYPE_SPEED:
	{
		m_currentWaveSprite = m_spriteSpeedRound;
		m_currentWaveSprite->SetEnabled(true);
		break;
	}
	case MAPTYPE_STRIKER:
	{
		m_currentWaveSprite = m_spriteStrikerRound;
		m_currentWaveSprite->SetEnabled(true);
		break;
	}
	case MAPTYPE_SLAYER:
	{
		m_currentWaveSprite = m_spriteSlayerRound;
		m_currentWaveSprite->SetEnabled(true);
		break;
	}
	default:
	{
		m_currentWaveSprite = nullptr;
		break;
	}
	}
	m_waveSpriteTick = 0;
}

void World::SpawnRow(MapRow row)
{
	for each (Entity* ent in row.m_rowEntities)
	{
		if (!m_isHostWorld)
			ent->m_sprite->SetEnabled(true);
		ent->m_pos.y +=  g_spriteRenderSystem->GetVirtualBounds().m_maxs.y - ent->m_bounds.m_mins.y;
		ent->m_sprite->m_position = ent->m_pos;
		ent->m_bounds = ent->GetBounds();
		m_entitiesToAdd.push_back(ent);
	}
}

void World::MakeStars()
{
	m_starFrameCount++;
	if (m_starFrameCount >= STAR_FRAME_FREQUENCY)
	{
		m_starFrameCount = 0;
		//Make a Star

		Star* newStar = new Star();
		Vector2 xBounds = Vector2(newStar->m_bounds.m_mins.x, newStar->m_bounds.m_maxs.x);
		Vector2 yBounds = Vector2(newStar->m_bounds.m_mins.y, newStar->m_bounds.m_maxs.y);
		float width = xBounds.y - xBounds.x;
		float height = yBounds.y - yBounds.x;

		float minX = g_spriteRenderSystem->GetVirtualBounds().m_mins.x + width;
		float maxX = g_spriteRenderSystem->GetVirtualBounds().m_maxs.x - width;
		float xSpawn = GetRandomFloatBetween(minX, maxX);
		float ySpawn = g_spriteRenderSystem->GetVirtualBounds().m_maxs.y + height;

		newStar->m_pos = Vector2(xSpawn, ySpawn);
		newStar->m_sprite->m_position = newStar->m_pos;
		newStar->m_bounds = newStar->GetBounds();

		m_entitiesBackGroundToAdd.push_back(newStar);
	}
}


NetObject* World::NetSyncObject(void *obj_ptr, uint32_t const net_obj_type_id, NetPlayerData *owner)
{
	// Get the type.
	NetObjectType *type = g_netTypesSystem->FindNetObjectType((eNetObjectTypes)net_obj_type_id);
	if (type == nullptr)
	{
		return nullptr;
	}
	

	NetObject *net_obj = new NetObject();

	// This MUST have a unique ID, you can use a incrementing counter, but make
	// sure that another object isn't using an ID before returning it.
	net_obj->net_id = NetGenerateNetworkID();
	net_obj->type = type;

	// Set it up.
	net_obj->owning_player_index = INVALID_CONNECTION_INDEX;
	net_obj->owning_connection_index = INVALID_CONNECTION_INDEX;
	if (nullptr != owner) {
		net_obj->owning_player_index = owner->m_playerIndex;
		net_obj->owning_connection_index = owner->m_ownerConnectionIndex;
	}

	net_obj->game_object = obj_ptr;

	// Register it - store it somewhere so we can find it by 
	// network_id.  std::map is actually not terrible here!
	RegisterNetObject(net_obj);


	// Send a packet telling everyone
	NetMessage create(eMessageID_NET_GAME_OBJECT_CREATE);

	create.m_definition = g_netSession->FindDefinition(create.m_messageType);
	create.WriteToMessage<uint16_t>(&net_obj->net_id);
	uint32_t typeID = net_obj_type_id;
	create.WriteToMessage<uint32_t>(&typeID);
	create.WriteToMessage<uint8_t>(&net_obj->owning_player_index);
	create.WriteToMessage<uint8_t>(&net_obj->owning_connection_index);
	type->SendCreate(net_obj, &create);
	create.FinalizeMessage();
	g_netSession->SendMessageToAllConnections(create);

	// return the net object (usually not used - though some people
	// like to keep a NetObject pointer on the object as well
	// as it makes cleanup faster.
	return net_obj;
	
}



void World::RegisterNetObject(NetObject* net_obj)
{
	m_netObjects.insert(std::pair<uint16_t, NetObject*>(net_obj->net_id, net_obj));
}

void World::OnProcessObjectCreate(NetSender const &from, NetMessage &msg)
{
	uint16_t net_id;
	uint32_t type_id;
	uint8_t ownerPlayerIndex;
	uint8_t ownerConnectionIndex;
	msg.ResetOffset();
	msg.m_offset = msg.GetHeaderSize();

	msg.Read<uint16_t>(&net_id);
	msg.Read<uint32_t>(&type_id);
	msg.Read<uint8_t>(&ownerPlayerIndex);
	msg.Read<uint8_t>(&ownerConnectionIndex);



	NetObjectType *type = g_netTypesSystem->FindNetObjectType((eNetObjectTypes)type_id);
	if (type == nullptr)
	{
		return;
	}

	if (DoesObjectOfNetIDExist(net_id))
	{
		//Object already exists, do not make.
		return;
	}

	NetObject *net_obj = new NetObject();

	// This MUST have a unique ID, you can use a incrementing counter, but make
	// sure that another object isn't using an ID before returning it.
	net_obj->net_id = net_id;
	net_obj->type = type;
	net_obj->type_id = type_id;

	
	// Set it up.
	net_obj->owning_player_index = ownerPlayerIndex;
	net_obj->owning_connection_index = ownerConnectionIndex;


	net_obj->game_object = type->OnCreate(&msg);
	AddObjectToWorld(net_obj, type_id);

	// Register it - store it somewhere so we can find it by 
	// network_id.  std::map is actually not terrible here!
	RegisterNetObject(net_obj);
}

void World::OnProcessObjectDestroy(NetSender const &from, NetMessage &msg)
{
	uint16_t net_id;
	uint32_t type_id;
	uint8_t ownerPlayerIndex;
	uint8_t ownerConnectionIndex;
	msg.ResetOffset();
	msg.m_offset = msg.GetHeaderSize();

	msg.Read<uint16_t>(&net_id);
	msg.Read<uint32_t>(&type_id);
	msg.Read<uint8_t>(&ownerPlayerIndex);
	msg.Read<uint8_t>(&ownerConnectionIndex);



	NetObjectType *type = g_netTypesSystem->FindNetObjectType((eNetObjectTypes)type_id);
	if (type == nullptr)
	{
		return;
	}

	if (!DoesObjectOfNetIDExist(net_id))
	{
		return;
	}

	auto iter = m_netObjects.find(net_id);
	if (iter == m_netObjects.end())
		return;

	NetObject *obj = m_netObjects.find(net_id)->second;

	RemoveObjectFromWorld(obj, type_id);
	delete obj;

	m_netObjects.erase(m_netObjects.find(net_id));

}

bool World::DoesObjectOfNetIDExist(uint16_t netID)
{
	if (m_netObjects.size() == 0)
	{
		return false;
	}
	auto iter = m_netObjects.find(netID);
	if (iter == m_netObjects.end())
		return false;
	NetObject *obj = m_netObjects.find(netID)->second;

	if (obj == nullptr)
	{
		return false;
	}
	return true;
}

void World::AddObjectToWorld(NetObject* obj, uint32_t typeID)
{
	if (typeID == eNetObjectTypes_COIN)
	{
		((Coin*)obj->game_object)->m_netObj = obj;
		SetupCoin((Coin*)obj->game_object);

	}
}

void World::RemoveNetObjAndSignalOthers(uint16_t netID)
{
	auto iter = m_netObjects.find(netID);
	if (iter == m_netObjects.end())
		return;

	NetObject *obj = m_netObjects.find(netID)->second;

	if (obj->owning_connection_index == g_netSession->m_myConnection->m_index ||
		(m_isHostWorld && obj->owning_connection_index == INVALID_CONNECTION_INDEX))
	{
		NetMessage destroy(eMessageID_NET_GAME_OBJECT_DESTROY);

		destroy.m_definition = g_netSession->FindDefinition(destroy.m_messageType);
		destroy.WriteToMessage<uint16_t>(&obj->net_id);
		destroy.WriteToMessage<uint32_t>(&obj->type_id);
		destroy.WriteToMessage<uint8_t>(&obj->owning_player_index);
		destroy.WriteToMessage<uint8_t>(&obj->owning_connection_index);
		//SendDestroy?
		destroy.FinalizeMessage();
		g_netSession->SendMessageToAllConnections(destroy);
	}
	


	RemoveObjectFromWorld(obj, obj->type_id);
	delete obj;

	m_netObjects.erase(m_netObjects.find(netID));
}

void World::RemoveObjectFromWorld(NetObject* obj, uint32_t typeID)
{
	if (typeID == eNetObjectTypes_COIN)
	{
		Coin* coin = (Coin*)obj->game_object;
		

		auto iter = m_coinList.begin();
		while (iter != m_coinList.end())
		{
			Coin* foundCoin = *iter;
			if (foundCoin->m_netObj->net_id == coin->m_netObj->net_id)
			{
				iter = m_coinList.erase(iter);
				delete coin;
				coin = nullptr;
				break;
			}
			else
			{
				++iter;
			}

		}

	}
}
void World::SetupCoin(Coin * coin)
{
	m_coinList.push_back(coin);
}

void World::HostCreateCoin()
{
	if (m_coinList.size() >= 3)
	{
		return;
	}

	float xRand = GetRandomFloatBetween(g_spriteRenderSystem->GetVirtualBounds().m_mins.x, g_spriteRenderSystem->GetVirtualBounds().m_maxs.x);
	float yRand = GetRandomFloatBetween(g_spriteRenderSystem->GetVirtualBounds().m_mins.y, g_spriteRenderSystem->GetVirtualBounds().m_maxs.y);

	Coin *coin = new Coin(Vector2(xRand, yRand));
	coin->m_sprite->SetEnabled(true);
	coin->m_sprite->SetEnabled(false);
	//coin->m_sprite->m_color = Rgba::GREEN;
	SetupCoin(coin);

	coin->m_netObj = NetSyncObject(coin, eNetObjectTypes_COIN, nullptr);
}

void World::NetObjectUpdate(NetConnection *cp)
{
	// This is simple for now, since most initial games
	// are are simple enough that this will work.
	// But this function tends to be where a bulk of
	// your time will go when your game gets more complex
	// optimizing it and prioritizing updates.

	// Nother optimization is putting all updates in the SAME packet, and sizing
	// it to be the size of the reamining room in your packet.
	auto mapIter = m_netObjects.begin();
	while (mapIter != m_netObjects.end())
	{
		NetObject *obj = mapIter->second;
		NetMessage* update = new NetMessage(eMessageID_NET_GAME_OBJECT_UPDATE);
		update->m_definition = g_netSession->FindDefinition(update->m_messageType);
		update->WriteToMessage<uint16_t>(&obj->net_id);
		if (m_isHostWorld)
		{
			obj->type->SendHostUpdate(obj, update);
		}
		else
		{
			obj->type->SendClientUpdate(obj, update);
		}
		update->FinalizeMessage();

		cp->AddMessage(update);

		++mapIter;
	}

}

void World::OnProcessObjectUpdateHost(NetSender const &from, NetMessage &msg)
{
	uint16_t id;
	msg.ResetOffset();
	msg.m_offset = msg.GetHeaderSize();

	msg.Read<uint16_t>(&id);

	auto iter = m_netObjects.find(id);
	if (iter == m_netObjects.end())
		return;

	NetObject *obj = m_netObjects.find(id)->second;
	if (obj != nullptr) {
		obj->type->ProcessHostUpdate(obj, &msg);
	}
}

void World::OnProcessObjectUpdateClient(NetSender const &from, NetMessage &msg)
{
	uint16_t id;
	msg.ResetOffset();
	msg.m_offset = msg.GetHeaderSize();

	msg.Read<uint16_t>(&id);


	auto iter = m_netObjects.find(id);
	if (iter == m_netObjects.end())
		return;

	NetObject *obj = m_netObjects.find(id)->second;
	if (obj != nullptr) {
		obj->type->ProcessClientUpdate(obj, &msg);
	}
}

void World::OnPlayerJoin(const NetSender &from, NetMessage &msg)
{
	msg.ResetOffset();
	msg.m_offset = msg.GetHeaderSize();

	NetPlayerData incomingPlayerData;

	incomingPlayerData.ReadPlayerDataFromMessage(&msg);

	NetPlayerData* newPlayer = nullptr;

	if (incomingPlayerData.m_ownerConnectionIndex == from.session->m_myConnection->m_index)
	{
		//Get MY player data via Nounce
		newPlayer = FindMyPlayerDataByNounce(incomingPlayerData.m_creationNuonce);
		//newPlayer = g_theGame->FindPlayerDataByConnectionIndex(incomingPlayerData.m_ownerConnectionIndex);


		if (newPlayer == nullptr)
		{
			return;
		}
	}
	else
	{
		//Make a New player for the other guy
		newPlayer = FindMyPlayerDataByNounce(incomingPlayerData.m_creationNuonce);

		if (newPlayer != nullptr)
		{
			return;
		}
		newPlayer = CreatePlayerData();

	}


	//copy the read in data to this player

	newPlayer->m_creationNuonce = incomingPlayerData.m_creationNuonce;
	newPlayer->m_ownerConnectionIndex = incomingPlayerData.m_ownerConnectionIndex;
	newPlayer->m_playerIndex = incomingPlayerData.m_ownerConnectionIndex;
	//AssignPlayerToIndex
	Player* player = AddPlayerAtIndex(newPlayer->m_ownerConnectionIndex);
	if (player != nullptr)
	{
		std::string joinStr = "Player Joined At: " + std::to_string(newPlayer->m_ownerConnectionIndex);
		g_theConsole->ConsolePrint(joinStr);
	}
}
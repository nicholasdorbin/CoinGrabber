#pragma once

#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Core/Camera3D.hpp"
#include "Game/GameCommon.hpp"

class Sprite;
class Player;
class NetObject;

const float PLAYER_HEIGHT = 1.85f;
const float PLAYER_WIDTH = 0.6f;

class Coin
{
public:
	Coin();
	Coin(Vector2 position);
	~Coin();

	NetObject* m_netObj;

	Sprite* m_sprite;
	Vector2 m_pos;
	Vector2 m_initialPos;

	float m_physicsRadius;
	AABB2 m_bounds;

	bool m_isDead;

	void Update(float deltaSeconds);
	void MoveAndUpdateBounds(Vector2 position);
	bool HandleCollisionWithPlayer(Player* player);
	void SetPhysicsRadius();
	AABB2 GetBoundsForPos(Vector2 position);
};

#pragma once
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Core/Camera3D.hpp"
#include "Game/Net/NetGameObjectParent.hpp"
#include "Game/GameCommon.hpp"
//#include "Engine/Audio/Audio.hpp"

class Sprite;

class Player : public NetGameObjectParent
{
public:
	Player();
	Player(Vector2 position);
	~Player();

	Sprite* m_sprite;

	Vector2 m_pos;
	Vector2 m_actualPos;
	Vector2 m_desiredPos;
	eGame_PlayerDirection m_direction;


	float m_moveMod;
	float m_moveModUp;
	float m_moveModRight;
	float m_age;
	float m_nextShootTick;
	float m_physicsRadius;
	AABB2 m_bounds;

	bool m_isMovingForward;
	bool m_isMovingBackwards;
	bool m_isMovingLeft;
	bool m_isMovingRight;
	bool m_isShooting;
	bool m_isDead;

	int m_health;
	uint16_t m_score;



	void Update(float deltaSeconds);
	void ClearMoveStates();
	void SetColorFromPlayerIndex(uint8_t index);

	void LerpToDesiredPos();
	void MoveAndUpdateBounds(Vector2 position);
	void PredictMovement();
	void ShiftPlayerIntoBounds();
	float GetPercentHealthRemaining();



	void UpdateCamera();
	void SetPhysicsRadius();


	AABB2 GetBoundsForPos(Vector2 position);
};
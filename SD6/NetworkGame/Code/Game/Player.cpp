#include "Game/Player.hpp"
#include "Game/TheGame.hpp"
#include "Game/TheApp.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Renderer/SpriteRenderer/Sprite.hpp"
#include "Engine/Renderer/SpriteRenderer/SpriteGameRenderer.hpp"
#include "Game/Bullet.hpp"

const float MOVE_SPEED_X = 8.f; //Blocks per Seconds
const float MOVE_SPEED_Y = 5.f; //Blocks per Seconds
const float VERT_MOD = 1.5f;

const float PLAYER_HEIGHT = 1.85f;
const float PLAYER_WIDTH = 0.6f;
const float PLAYER_EYE_HEIGHT = 1.62f;
const float FIRE_RATE = 0.1f;
const int PLAYER_BULLET_DAMAGE = 1;
const int PLAYER_HEALTH = 20;

const Vector2 SPRITE_RIGHT = Vector2(1.f, 0.f);
const Vector2 SPRITE_FORWARD = Vector2(0.f, 1.f);

Player::Player():
	m_direction(eGame_PlayerDirection_NORTH)
	, m_score(0)
{
	m_pos = Vector2(0.f, 0.f);

	m_actualPos = m_pos;
	m_desiredPos = m_pos;

	
	m_moveMod = 1.f;
	m_moveModUp = 1.f;
	m_moveModRight = 1.f;
	m_age = 0.f;
	m_nextShootTick = 0.f;
	m_physicsRadius = 1.f;


	m_isMovingForward = false;
	m_isMovingBackwards = false;
	m_isMovingLeft = false;
	m_isMovingRight = false;
	m_isShooting = false;
	m_isDead = false;

	m_health = PLAYER_HEALTH;

	m_sprite = Sprite::Create("hand");
	m_sprite->m_scale = Vector2(0.1f, 0.1f);

	Vector2 mins = Vector2(m_pos.x - (PLAYER_WIDTH / 2.f), m_pos.y - (PLAYER_WIDTH / 2.f));
	Vector2 maxs = Vector2(m_pos.x + (PLAYER_WIDTH / 2.f), m_pos.y + (PLAYER_WIDTH / 2.f));
	m_bounds = AABB2(mins, maxs);
	SetPhysicsRadius();
}

Player::Player(Vector2 position):
	m_direction(eGame_PlayerDirection_NORTH)
	, m_score(0)
{
	m_pos = position;
	m_actualPos = m_pos;
	m_desiredPos = m_pos;

	m_moveMod = 1.f;
	m_moveModUp = 1.f;
	m_moveModRight = 1.f;
	m_physicsRadius = 1.f;

	m_isMovingForward = false;
	m_isMovingBackwards = false;
	m_isMovingLeft = false;
	m_isMovingRight = false;
	m_isShooting = false;
	m_isDead = false;
	m_age = 0.f;
	m_nextShootTick = 0.f;

	m_sprite = Sprite::Create("hand");
	m_sprite->m_scale = Vector2(0.1f, 0.1f);

	Vector2 mins = Vector2(m_pos.x - (PLAYER_WIDTH / 2.f), m_pos.y - (PLAYER_WIDTH / 2.f));
	Vector2 maxs = Vector2(m_pos.x + (PLAYER_WIDTH / 2.f), m_pos.y + (PLAYER_WIDTH / 2.f));
	m_bounds = AABB2(mins, maxs);

	m_health = PLAYER_HEALTH;
	SetPhysicsRadius();
}

Player::~Player()
{
	delete m_sprite;
}

void Player::Update(float deltaSeconds)
{
	m_age += deltaSeconds;
	MoveAndUpdateBounds(m_pos);

	if (m_health <= 0)
	{
		m_isDead = true;
	}


	if (!m_isDead)
	{
		bool isMoving = false;
		if (m_isMovingForward)
		{
			m_direction = eGame_PlayerDirection_NORTH;
			m_pos += SPRITE_FORWARD * MOVE_SPEED_Y * m_moveModUp * deltaSeconds;
			isMoving = true;
		}
		else if (m_isMovingBackwards)
		{
			m_direction = eGame_PlayerDirection_SOUTH;
			m_pos += SPRITE_FORWARD * MOVE_SPEED_Y * m_moveModUp * deltaSeconds;
			isMoving = true;
		}
		if (m_isMovingLeft)
		{
			m_direction = eGame_PlayerDirection_WEST;
			m_pos += SPRITE_RIGHT * MOVE_SPEED_X * m_moveModRight * deltaSeconds;
			isMoving = true;
		}
		else if (m_isMovingRight)
		{
			m_direction = eGame_PlayerDirection_EAST;
			m_pos += SPRITE_RIGHT * MOVE_SPEED_X * m_moveModRight * deltaSeconds;
			isMoving = true;
		}

		if (!isMoving)
		{
			m_direction = eGame_PlayerDirection_NOT_MOVING;
		}
		MoveAndUpdateBounds(m_pos);


		if (m_isShooting && m_nextShootTick < m_age)
		{
// 			m_nextShootTick = m_age + FIRE_RATE;
// 
// 			Vector2 upDisp = Vector2(m_pos.x, m_bounds.m_maxs.y);
// 			Bullet* newBullet = new Bullet(upDisp, 90.f, false, PLAYER_BULLET_DAMAGE);
// 
// 			g_theGame->m_world->m_entitiesToAdd.push_back(newBullet);
			// 		Bullet* newBullet = new Bullet(m_pos, m_orientation, false, m_bulletDamage);
			// 		newBullet->m_pos.x += (m_cosmeticRadius * DegToRad(cos(m_orientation)));
			// 		newBullet->m_pos.y += (m_cosmeticRadius * DegToRad(sin(m_orientation)));
			// 		m_bullets.push_back(newBullet);
			// 
			// 		SoundID sound = g_theGame->m_audio->CreateOrGetSound(PLAYER_SHOOT_SOUND);
			// 		g_theGame->m_audio->PlaySound(sound, 0.7f);
		}
	}
	
	
	
}


void Player::ClearMoveStates()
{
	m_isMovingForward = false;
	m_isMovingBackwards = false;
	m_isMovingLeft = false;
	m_isMovingRight = false;
}

void Player::SetColorFromPlayerIndex(uint8_t index)
{
	if (index == 0)
	{
		m_sprite->m_color = Rgba::WHITE;
	}
	else if (index == 1)
	{
		m_sprite->m_color = Rgba::BLUE;
	}
	else if (index == 2)
	{
		m_sprite->m_color = Rgba::YELLOW;
	}
	else if (index == 3)
	{
		m_sprite->m_color = Rgba::RED;
	}
	else if (index == 4)
	{
		m_sprite->m_color = Rgba::LTBLUE;
	}
	else if (index == 5)
	{
		m_sprite->m_color = Rgba::LTGRAY;
	}
	else if (index == 6)
	{
		m_sprite->m_color = Rgba::ORANGE_RED;
	}
	else if (index == 7)
	{
		m_sprite->m_color = Rgba::PINK;
	}
	else
	{
		m_sprite->m_color = Rgba::PURPLE;
	}
}

void Player::LerpToDesiredPos()
{
	m_pos = (m_pos*(0.8f) + m_desiredPos * (0.2f));
}

void Player::MoveAndUpdateBounds(Vector2 position)
{
	m_pos = position;
	m_sprite->m_position = m_pos;
	m_bounds = GetBoundsForPos(m_pos);

	switch (m_direction)
	{
	case eGame_PlayerDirection_NORTH:
	{
		m_sprite->m_rotationDegrees = 0;
		break;
	}
	case eGame_PlayerDirection_SOUTH:
	{
		m_sprite->m_rotationDegrees = 180.f;
		break;
	}
	case eGame_PlayerDirection_EAST:
	{
		m_sprite->m_rotationDegrees = 270.f;
		break;
	}
	case eGame_PlayerDirection_WEST:
	{
		m_sprite->m_rotationDegrees = 90.f;
		break;
	}
	{
	default:
		break;
	}
	}
	ShiftPlayerIntoBounds();

}

void Player::PredictMovement()
{
	
	m_sprite->m_position = m_pos;
	m_bounds = GetBoundsForPos(m_pos);

	switch (m_direction)
	{
	case eGame_PlayerDirection_NORTH:
	{
		m_sprite->m_rotationDegrees = 0;
		break;
	}
	case eGame_PlayerDirection_SOUTH:
	{
		m_sprite->m_rotationDegrees = 180.f;
		break;
	}
	case eGame_PlayerDirection_EAST:
	{
		m_sprite->m_rotationDegrees = 270.f;
		break;
	}
	case eGame_PlayerDirection_WEST:
	{
		m_sprite->m_rotationDegrees = 90.f;
		break;
	}
	{
	default:
		break;
	}
	}
	ShiftPlayerIntoBounds();
}

void Player::ShiftPlayerIntoBounds()
{
	AABB2 screenBounds = g_spriteRenderSystem->GetVirtualBounds();
	Vector2 diff = Vector2::ZERO;
	if (m_bounds.m_mins.x < screenBounds.m_mins.x)
	{
		diff.x = screenBounds.m_mins.x - m_bounds.m_mins.x;
	}
	else if (m_bounds.m_maxs.x > screenBounds.m_maxs.x)
	{
		diff.x = screenBounds.m_maxs.x - m_bounds.m_maxs.x;
	}


	if(m_bounds.m_mins.y < screenBounds.m_mins.y)
	{
		diff.y = screenBounds.m_mins.y - m_bounds.m_mins.y;
	}
	else if (m_bounds.m_maxs.y > screenBounds.m_maxs.y)
	{
		diff.y = screenBounds.m_maxs.y - m_bounds.m_maxs.y;
	}

	m_pos += diff;
	m_sprite->m_position = m_pos;
	m_bounds = GetBoundsForPos(m_pos);
}

float Player::GetPercentHealthRemaining()
{

	return (float)m_health / (float)PLAYER_HEALTH * 100.f;
}

void Player::SetPhysicsRadius()
{
	float highestDimension = m_sprite->GetHeight() * m_sprite->m_scale.y;
	float spriteWidth = m_sprite->GetWidth() * m_sprite->m_scale.x;
	if (highestDimension < spriteWidth)
		highestDimension = spriteWidth;

	m_physicsRadius = highestDimension / 2.f;
}

AABB2 Player::GetBoundsForPos(Vector2 position)
{
	position;
	//Vector2 mins = Vector2(m_pos.x - (PLAYER_WIDTH / 2.f), m_pos.y - (PLAYER_WIDTH / 2.f));
	//Vector2 maxs = Vector2(m_pos.x + (PLAYER_WIDTH / 2.f), m_pos.y + (PLAYER_WIDTH / 2.f));
	Vector2 mins = m_sprite->GetSpriteBottomLeft();
	Vector2 maxs = m_sprite->GetSpriteTopRight();
	return AABB2(mins, maxs);
}


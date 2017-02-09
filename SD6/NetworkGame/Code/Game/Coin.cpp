#include "Game/Coin.hpp"
#include "Engine/Renderer/SpriteRenderer/Sprite.hpp"
#include "Engine/Renderer/SpriteRenderer/SpriteGameRenderer.hpp"
#include "Engine/MathUtils.hpp"
#include "Game/Player.hpp"
#include "Game/Net/NetObject.hpp"


Coin::Coin() :
	m_isDead(false)
{
	m_pos = Vector2(0.f, 0.f);
	m_initialPos = m_pos;
	m_physicsRadius = 1.f;
	m_sprite = Sprite::Create("coin");
	m_sprite->m_scale = Vector2(0.1f, 0.1f);
	Vector2 mins = Vector2(m_pos.x - (PLAYER_WIDTH / 2.f), m_pos.y - (PLAYER_WIDTH / 2.f));
	Vector2 maxs = Vector2(m_pos.x + (PLAYER_WIDTH / 2.f), m_pos.y + (PLAYER_WIDTH / 2.f));
	m_bounds = AABB2(mins, maxs);
	SetPhysicsRadius();
}

Coin::Coin(Vector2 position) :
	m_isDead(false)
{
	m_pos = position;
	m_initialPos = m_pos;
	m_physicsRadius = 1.f;



	m_sprite = Sprite::Create("coin");
	m_sprite->m_scale = Vector2(0.1f, 0.1f);

	Vector2 mins = Vector2(m_pos.x - (PLAYER_WIDTH / 2.f), m_pos.y - (PLAYER_WIDTH / 2.f));
	Vector2 maxs = Vector2(m_pos.x + (PLAYER_WIDTH / 2.f), m_pos.y + (PLAYER_WIDTH / 2.f));
	m_bounds = AABB2(mins, maxs);

	SetPhysicsRadius();
}

Coin::~Coin()
{
	delete m_sprite;
}

void Coin::Update(float deltaSeconds)
{

	if (m_isDead)
	{
		m_sprite->SetEnabled(false);
		return;
	}

	float xRand = GetRandomFloatBetween(0.15f, 0.25f);
	float yRand = GetRandomFloatBetween(0.15f, 0.25f);

	m_pos = Vector2(m_initialPos.x + xRand, m_initialPos.y + yRand);

	

	MoveAndUpdateBounds(m_pos);
}

void Coin::MoveAndUpdateBounds(Vector2 position)
{
	m_pos = position;
	m_sprite->m_position = m_pos;
	m_bounds = GetBoundsForPos(m_pos);
}

bool Coin::HandleCollisionWithPlayer(Player* player)
{
	if (m_isDead)
		return false;

	Vector2 playerPos = player->m_pos;
	float playerRadius = player->m_physicsRadius;
	if (DoDiscsOverlap(m_pos, m_physicsRadius, playerPos, playerRadius))
	{
		m_isDead = true;
		
		return true;
	}
	return false;
}

void Coin::SetPhysicsRadius()
{
	float highestDimension = m_sprite->GetHeight() * m_sprite->m_scale.y;
	float spriteWidth = m_sprite->GetWidth() * m_sprite->m_scale.x;
	if (highestDimension < spriteWidth)
		highestDimension = spriteWidth;

	m_physicsRadius = highestDimension / 2.f;
}

AABB2 Coin::GetBoundsForPos(Vector2 position)
{
	position;
	//Vector2 mins = Vector2(m_pos.x - (PLAYER_WIDTH / 2.f), m_pos.y - (PLAYER_WIDTH / 2.f));
	//Vector2 maxs = Vector2(m_pos.x + (PLAYER_WIDTH / 2.f), m_pos.y + (PLAYER_WIDTH / 2.f));
	Vector2 mins = m_sprite->GetSpriteBottomLeft();
	Vector2 maxs = m_sprite->GetSpriteTopRight();
	return AABB2(mins, maxs);
}

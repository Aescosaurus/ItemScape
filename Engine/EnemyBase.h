#pragma once

#include "Vec2.h"
#include "Collider.h"
#include "TileMap.h"
#include "Codex.h"
#include <memory>
#include <vector>
#include "Anim.h"
#include "Graphics.h"
#include "Surface.h"
#include "SpriteEffect.h"
#include "Timer.h"

class EnemyUpdateInfo
{
public:
	const Vec2& playerPos;
	const Vec2& playerVel;
};

// TODO: Add a boss base someday maybe.
class EnemyBase
{
public:
	EnemyBase() = delete;

	// Make sure to call this from Child::Update.
	virtual void Update( const EnemyUpdateInfo& info,float dt );
	virtual void Draw( Graphics& gfx ) const = 0;

	// Make sure to call this from Child::Attack.
	virtual void Attack( int damage,const Vec2& loc );

	bool IsExpl() const;
	const Vec2& GetPos() const;
	const Rect& GetRect() const;
	// Returns hp percent 0-1.
	float GetHPPercent() const;
protected:
	EnemyBase( const Vec2& pos,const Vec2& size,
		int hp,const TileMap& map );

	void Wander( float moveTolerance,float speed,float dt );
	void ResetTargeting( float moveTolerance,float speed );
	void AttemptMove( float dt );

	SpriteEffect::Substitution FlashCol() const;
	bool IsFlashing() const;
protected:
	Vec2 pos;
	Collider coll;
	int maxHP;
	int hp;
	const TileMap* map;
	Vec2 target = { 0.0f,0.0f };
	Vec2 lastTarget = pos;
	Vec2 vel = { 0.0f,0.0f };
	Timer damageCooldown = 3.0f / 60.0f;
	bool reachedTarget = false;
};
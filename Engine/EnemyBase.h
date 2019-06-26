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
#include "Door.h"
#include "Bullet.h"

class EnemyUpdateInfo
{
public:
	const Vec2& playerPos;
	const Vec2& playerVel;
	std::vector<Door>& doors;
	std::vector<std::unique_ptr<Bullet>>& enemyBullets;
};

// TODO: Add a boss base someday maybe.
class EnemyBase
{
public:
	enum class Effect
	{
		Frozen, // Can't move, shoot, or do anything.
		Stunned, // No shooting.
		Stuck, // No moving.
		Confused // Bullets go in random directions.
	};
public:
	EnemyBase() = delete;

	void UpdateBase( const EnemyUpdateInfo& info,float dt );
	// Make sure to call this from Child::Update.
	virtual void Update( const EnemyUpdateInfo& info,float dt );
	virtual void Draw( Graphics& gfx ) const = 0;

	// Make sure to call this from Child::Attack.
	virtual void Attack( int damage,const Vec2& loc );
	void ApplyEffect( Effect eff,float duration );

	bool IsExpl() const;
	const Vec2& GetPos() const;
	const Rect& GetRect() const;
	// Returns hp percent 0-1.
	float GetHPPercent() const;
	int GetCurHP() const;
	int GetMaxHP() const;
	virtual bool IsBoss() const;
protected:
	EnemyBase( const Vec2& pos,const Vec2& size,
		int hp,const TileMap& map );

	void Wander( float moveTolerance,float speed,float dt );
	void ResetTargeting( float moveTolerance,float speed );
	void AttemptMove( float dt );

	SpriteEffect::Substitution FlashCol() const;
	bool IsFlashing() const;
	bool HasEffect( Effect eff ) const;
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
	static constexpr float damagePushDist = 11.0f;
	std::vector<std::pair<Effect,Timer>> effects;
};
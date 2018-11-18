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

	virtual void Update( const EnemyUpdateInfo& info,float dt ) = 0;
	virtual void Draw( Graphics& gfx ) const = 0;

	virtual void Attack( int damage,const Vec2& loc );

	bool IsExpl() const;
	const Rect& GetRect() const;
protected:
	EnemyBase( const Vec2& pos,const Vec2& size,
		int hp,const TileMap& map );

	void Wander( float moveTolerance,float speed,float dt );
	void ResetTargeting( float moveTolerance,float speed );
protected:
	Vec2 pos;
	Collider coll;
	int hp;
	const TileMap* map;
	Vec2 target = { 0.0f,0.0f };
	Vec2 lastTarget = pos;
	Vec2 vel = { 0.0f,0.0f };
};
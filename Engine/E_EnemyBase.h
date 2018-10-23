#pragma once

#include "Vec2.h"
#include "Collider.h"
#include "TileMap.h"

// TODO: Add a boss base someday maybe.
class EnemyBase
{
public:
	EnemyBase() = delete;

	virtual void Update( const Vec2& playerPos,float dt ) = 0;
	virtual void Draw( Graphics& gfx ) const = 0;

	virtual void Attack( int damage,const Vec2& sourcePos );
protected:
	EnemyBase( const Vec2& pos,const Vec2& size,
		int hp,const TileMap& map );

	void TryMovement( const Vec2& testMove );
protected:
	Vec2 pos;
	Collider coll;
	int hp;
	const TileMap* map;
};
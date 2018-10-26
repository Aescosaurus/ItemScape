#pragma once

#include "Vec2.h"
#include "Graphics.h"
#include "Codex.h"
#include "Anim.h"
#include "TileMap.h"
#include "Collider.h"

class Bullet
{
public:
	enum class Team
	{
		Player = 0,
		Beetle,
		LastOfSmall,
		BeetleBig
	};
	enum class Size
	{
		Small = 1,
		Medium = 2
	};
public:
	Bullet( const Vec2& pos,const Vec2& target,
		const TileMap& map,Team myTeam,float speed,Size mySize );

	void Update( float dt );
	void Draw( Graphics& gfx ) const;

	void Attack( int damage );

	bool IsDead() const;
	const Rect& GetRect() const;
private:
	const Surface* pSprSheet = Codx::Load( "Images/BulletAnims.bmp",{ 4,4 } );
	static constexpr Vei2 size = { 16,16 };
	Vec2 pos;
	Vec2 vel;
	Team myTeam;
	Size mySize;
	const TileMap* map;
	Anim myAnim;
	bool dead = false;
	Collider coll;
};
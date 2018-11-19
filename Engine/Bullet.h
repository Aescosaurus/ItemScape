#pragma once

#include "Vec2.h"
#include "Graphics.h"
#include "Codex.h"
#include "Anim.h"
#include "TileMap.h"
#include "Collider.h"

// Moves straight in one direction.
class Bullet
{
public:
	enum class Team
	{
		Player = 0,
		Beetle,
		Firebug,
		Skorp,
		RynoChaser,
		LastOfSmall,
		BeetleBig,
		SpiderMole
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

	bool IsExpl() const;
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

// Ideas:
//  -Boomerang that goes in a direction for a while then comes back.
//  -Boomerang2 goes in a circle-like motion to hit target then come back.
//  -Slither/soidal "slithers" back and forth towards target.
//  -Bubble bullet "floats" sort of randomly but slowly makes its way to the target/player.

#pragma once

#include "Vec2.h"
#include "Graphics.h"
#include "Codex.h"
#include "Anim.h"
#include "TileMap.h"

class Bullet
{
public:
	enum class Team
	{
		Player = 0,
		Beetle,
		BeetleBig
	};
public:
	Bullet( const Vec2& pos,const Vec2& target,
		const TileMap& map,Team myTeam,float mySpeed );

	void Update( float dt );
	void Draw( Graphics& gfx ) const;

	bool IsDead() const;
private:
	Vec2 pos;
	static constexpr Vei2 size = { 16,16 };
	Vec2 vel;
	Team myTeam;
	const Surface* pSprSheet = Codx::Load( "Images/BulletAnims.bmp",{ 4,4 } );
	Anim myAnim;
	const TileMap* map;
	bool dead = false;
};
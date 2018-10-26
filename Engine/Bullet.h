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
	virtual void Update( float dt );
	virtual void Draw( Graphics& gfx ) const = 0;

	bool IsDead() const;
protected:
	Bullet( const Vec2& pos,const Vec2& target,
		const TileMap& map,Team myTeam,float speed );
protected:
	Vec2 pos;
	Vec2 vel;
	Team myTeam;
	const TileMap* map;
	bool dead = false;
};

class BulletS
	:
	public Bullet
{
public:
	BulletS( const Vec2& pos,const Vec2& target,
		const TileMap& map,Team myTeam,float speed );

	void Update( float dt ) override;
	void Draw( Graphics& gfx ) const override;
private:
	static constexpr Vei2 size = { 16,16 };
	const Surface* pSprSheet = Codx::Load( "Images/BulletAnimsSmall.bmp",{ 4,4 } );
	Anim myAnim;
};

class BulletM
	:
	public Bullet
{
public:
	BulletM( const Vec2& pos,const Vec2& target,
		const TileMap& map,Team myTeam,float speed );

	void Update( float dt ) override;
	void Draw( Graphics& gfx ) const override;
private:
	static constexpr Vei2 size = { 32,32 };
	const Surface* pSprSheet = Codx::Load( "Images/BulletAnimsMedium.bmp",{ 4,4 } );
	Anim myAnim;
};
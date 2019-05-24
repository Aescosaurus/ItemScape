#pragma once

#include "Vec2.h"
#include "Graphics.h"
#include "Codex.h"
#include "Anim.h"
#include "TileMap.h"
#include "Collider.h"
#include "Timer.h"

class BulletUpdateInfo
{
public:
	std::vector<std::unique_ptr<class EnemyBase>>& enemies;
	float dt;
	class Player& player;
};

// Moves straight in one direction.
class Bullet
{
public:
	enum class Team
	{
		Player1 = 0,
		Player2,
		Player3,
		Player4,
		Beetle,
		Firebug,
		Skorp,
		RynoChaser,
		LastOfSmall, // 
		BeetleBig,
		SpiderMole,
		Slizard,
		WingedBug
	};
	enum class Size
	{
		Small = 1,
		Medium = 2
	};
public:
	Bullet( const Vec2& pos,const Vec2& target,
		const TileMap& map,Team myTeam,float speed,
		Size mySize,int damage = 1 );

	virtual void Update( BulletUpdateInfo& info );
	void Draw( Graphics& gfx ) const;

	void Attack( int damage );
	void SetSubColor( Color c );
	virtual Bullet* Clone();
	void SetVel( const Vec2& vel );

	bool IsExpl() const;
	Vec2& GetPos();
	const Rect& GetRect() const;
	int& GetDamage();
	Vec2& GetVel();
protected:
	// Bullet( const Vec2& pos,const Vec2& vel,Team myTeam,
	// 	Size mySize,const TileMap* map,Anim anim,
	// 	Collider coll,)
protected:
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
	Color subColor = Colors::Magenta;
	int damage;
};

class WavyBullet
	:
	public Bullet
{
public:
	WavyBullet( const Bullet& src );

	void Update( BulletUpdateInfo& info ) override;

	Bullet* Clone() override;
private:
	float distTravelled = 0.0f;
	Vec2 normVel;
	float speed;
	float freq = 15.0f;
	float amplitude = 0.7f;
};

class TrackingBullet
	:
	public Bullet
{
public:
	TrackingBullet( const Bullet& src );

	void Update( BulletUpdateInfo& info ) override;

	// void SetTarget( const Vec2& target );
	void SetSpeed( float speed );
	void SetOffset( const Vec2& offset );

	Bullet* Clone() override;
private:
	float speed;
	Vec2 offset = { 0.0f,0.0f };
	Timer targetTime = 0.3f;
};

// Ideas:
//  -Boomerang that goes in a direction for a while then comes back.
//  -Boomerang2 goes in a circle-like motion to hit target then come back.
//  -Slither/soidal "slithers" back and forth towards target.
//  -Bubble bullet "floats" sort of randomly but slowly makes its way to the target/player.

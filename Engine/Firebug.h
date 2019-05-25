#pragma once

#include "EnemyBase.h"
#include "Bullet.h"
#include "Timer.h"

class Firebug
	:
	public EnemyBase
{
private:
	enum class State
	{
		GotoPlayer,
		Wander,
		Charge,
		AttackLeft,
		AttackRight,
		Explode
	};
public:
	Firebug( const Vec2& pos,const TileMap& map,
		std::vector<std::unique_ptr<Bullet>>& bulletVec );

	void Update( const EnemyUpdateInfo& info,float dt ) override;
	void Draw( Graphics& gfx ) const override;

	void Attack( int damage,const Vec2& loc ) override;
private:
	// Angle is in radians.
	void ShootBullet( float angle );

	Vec2 GetCenter() const;
private:
	static constexpr Vei2 size = { 64,64 };
	static constexpr int myHP = 13;
	std::vector<std::unique_ptr<Bullet>>* pBulletVec;
	CSurfPtr sprSheet = SurfCodex::Fetch( "Images/Firebug.bmp" );
	Anim walking;
	Anim charging;
	Anim attacking;
	Anim exploding;
	State curAction = State::GotoPlayer;
	Timer moveStop = { 1.6f,true };
	Timer shotTimer = 0.1f;
	static constexpr int nShotsASide = 5;
	int curShot = 0;
	float shotAngle = 0.0f;
	Timer moveRetarget = { 0.4f,true };
	static constexpr float speed = 105.24f;
	static constexpr float bulletSpeed = 200.0f;
	static constexpr float moveTolerance = 163.4f;
};
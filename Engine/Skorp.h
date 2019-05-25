#pragma once

#include "EnemyBase.h"
#include "Bullet.h"
#include "Timer.h"
#include "Random.h"

class Skorp
	:
	public EnemyBase
{
private:
	enum class State
	{
		Walk,
		Wander,
		WindUp,
		Attack,
		Explode
	};
public:
	Skorp( const Vec2& pos,const TileMap& map,
		std::vector<std::unique_ptr<Bullet>>& bullets );

	void Update( const EnemyUpdateInfo& info,float dt ) override;
	void Draw( Graphics& gfx ) const override;

	void Attack( int damage,const Vec2& loc ) override;
private:
	Vec2 GetCenter() const;
private:
	static constexpr Vei2 size = { 64,64 };
	static constexpr int myHealth = 14;
	std::vector<std::unique_ptr<Bullet>>& bullets;
	CSurfPtr sprSheet = SurfCodex::Fetch( "Images/SkorpAnim.bmp" );
	Anim walking;
	Anim winding;
	Anim attacking;
	Anim exploding;
	State action = State::Wander; // Not walk.
	Timer moveStateChange = 0.97f;
	static constexpr float speed = 126.4f;
	Timer wanderDuration = { 1.14f,true };
	static constexpr float bulletSpeed = 215.4f;
	static constexpr float moveTolerance = 115.4f;
};
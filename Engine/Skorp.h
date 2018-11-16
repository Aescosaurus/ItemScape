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
	void ResetTargeting();

	Vec2 FindTarget() const;
	Vec2 GetCenter() const;
private:
	static constexpr Vei2 size = { 64,64 };
	static constexpr int myHealth = 14;
	std::vector<std::unique_ptr<Bullet>>& bullets;
	const Surface* sprSheet = Codx::Load( "Images/SkorpAnim.bmp",{ 4,4 } );
	Anim walking;
	Anim winding;
	Anim attacking;
	Anim exploding;
	State action = State::Wander; // Not walk.
	Vec2 vel = { 0.0f,0.0f };
	Vec2 target = pos;
	Vec2 lastTarget = target;
	Timer moveStateChange = 0.97f;
	static constexpr float speed = 126.4f;
	Timer wanderDuration = { 1.14f,true };
	static constexpr float bulletSpeed = 215.4f;
};
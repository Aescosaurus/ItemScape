#pragma once

#include "EnemyBase.h"
#include "Bullet.h"
#include "Timer.h"

class RynoChaser
	:
	public EnemyBase
{
private:
	enum class State
	{
		Wander,
		ChargeUp,
		ChargePlayer,
		Explode
	};
public:
	RynoChaser( const Vec2& pos,const TileMap& map,
		std::vector<std::unique_ptr<Bullet>>& bullets );

	void Update( const EnemyUpdateInfo& info,float dt ) override;
	void Draw( Graphics& gfx ) const override;

	void Attack( int damage,const Vec2& loc ) override;
private:
	Vec2 GetCenter() const;
private:
	static constexpr Vei2 size = { 64,64 };
	static constexpr int health = 17;
	std::vector<std::unique_ptr<Bullet>>* pBulletVec;
	State action = State::Wander;
	static constexpr float moveTolerance = 151.5f;
	static constexpr float speed = 43.1f;
	static constexpr float fastSpeed = 225.6f;
	Timer wanderPeriod = { 2.4f,true };
	const Surface* sprSheet = Codx::Load( "Images/RynoChaserAnim.bmp",{ 4,4 } );
	Anim wandering;
	Anim chargingUp;
	Anim chargingPlayer;
	Anim exploding;
	Timer timeBetweenShots = 0.2f;
	static constexpr float bulletSpeed = 62.1f;
	Timer attackDuration = 1.0f;
};
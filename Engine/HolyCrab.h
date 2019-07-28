#pragma once

#include "EnemyBase.h"

class HolyCrab
	:
	public EnemyBase
{
private:
	enum class State
	{
		Wander,
		WindUp,
		SpinAttack,
		Exploding
	};
public:
	HolyCrab( const Vec2& pos,const TileMap& map,
		std::vector<std::unique_ptr<Bullet>>& bulletVec );

	void Update( const EnemyUpdateInfo& info,float dt ) override;
	void Draw( Graphics& gfx ) const override;

	// void Attack( int damage,const Vec2& loc ) override;
private:
	static constexpr Vei2 size = { 64,64 };
	static constexpr int myHP = 35;
	std::vector<std::unique_ptr<Bullet>>* pBulletVec;
	CSurfPtr sprSheet = SurfCodex::Fetch( "Images/HolyCrab.bmp" );
	Anim wandering;
	Anim windingUp;
	Anim spinAttacking;
	Anim exploding;
	State action = State::Wander;
	static constexpr float moveRange = 100.0f;
	static constexpr float wanderSpeed = 60.0f;
	Timer wanderTimer = 2.0f;
	static constexpr int shotsPerVolley = 8;
	int curShot = 0;
	Timer shotTimer = 0.4f;
	static constexpr float bulletSpeed = 210.0f;
};
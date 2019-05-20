#pragma once

#include "EnemyBase.h"
#include "Bullet.h"
#include "Timer.h"

class SpiderMole
	:
	public EnemyBase
{
private:
	enum class State
	{
		Wander,
		Charge,
		Rise,
		Sink,
		Explode
	};
public:
	SpiderMole( const Vec2& pos,const TileMap& map,
		std::vector<std::unique_ptr<Bullet>>& bullets );

	void Update( const EnemyUpdateInfo& info,float dt ) override;
	void Draw( Graphics& gfx ) const override;

	void Attack( int damage,const Vec2& loc ) override;
private:
	void ResetTargeting();
	void SpawnBulletCircle();

	Vec2 FindTarget() const;
	Vec2 GetCenter() const;
private:
	static constexpr Vei2 size = { 64,64 };
	static constexpr int health = 7;
	std::vector<std::unique_ptr<Bullet>>* pBulletVec;
	const Surface* sprSheet = Codx::Load( "Images/SpiderMoleAnim.bmp",{ 4,4 } );
	Anim walking;
	Anim rising;
	Anim sinking;
	Anim exploding;
	State action = State::Wander;
	Vec2 target = pos;
	Vec2 lastTarget = target;
	Vec2 vel = { 0.0f,0.0f };
	static constexpr float speed = 90.1f;
	static constexpr float fastSpeed = 194.67f;
	Timer walkEnd = { 2.15f,true };
	static constexpr float bulletSpeed = 141.2f;
};
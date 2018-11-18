#pragma once

#include "EnemyBase.h"
#include "Bullet.h"

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
	static constexpr Vei2 size = { 64,64 };
	static constexpr int health = 17;
	std::vector<std::unique_ptr<Bullet>>* pBulletVec;
	State action = State::Wander;
};
#pragma once

#include "EnemyBase.h"
#include "Bullet.h"

class Slizard
	:
	public EnemyBase
{
private:
	enum class State
	{
		Wander,
		Charge,
		Attack,
		Explode
	};
public:
	Slizard( const Vec2& pos,const TileMap& map,
		std::vector<std::unique_ptr<Bullet>>& bullets );

	// Wander until you get line of sight to player, then wind up and attack.
	void Update( const EnemyUpdateInfo& info,float dt ) override;
	void Draw( Graphics& gfx ) const override;
private:
	static constexpr Vei2 size = { 64,64 };
	static constexpr int health = 9;
	std::vector<std::unique_ptr<Bullet>>* pBulletVec;
	const Surface* surfSheet = Codx::Load( "Images/SlizardAnim.bmp",{ 4,4 } );
	Anim walking;
	Anim charging;
	Anim attacking;
	Anim exploding;
	State action = State::Wander;
};
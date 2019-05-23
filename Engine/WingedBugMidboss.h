#pragma once

#include "EnemyBase.h"
#include "Bullet.h"

class WingedBugMidboss
	:
	public EnemyBase
{
private:
	enum class State
	{
		Stay,
		Wander,
		BuildUp,
		ChargeAttack,
		Explode
	};
public:
	WingedBugMidboss( const Vec2& pos,const TileMap& map,
		std::vector<std::unique_ptr<Bullet>>& bullets );

	void Update( const EnemyUpdateInfo& info,float dt ) override;
	void Draw( Graphics& gfx ) const override;

	void Attack( int damage,const Vec2& loc ) override;
private:
	static constexpr Vei2 size = { 128,128 };
	static constexpr int health = 250;
	std::vector<std::unique_ptr<Bullet>>* pBulletVec;
	const Surface* surfSheet = Codx::Load( "Images/WingedBugMidboss.bmp",Vei2{ 4,4 } );
	Anim floating;
	Anim buildingUp;
	Anim charging;
	Anim exploding;
	State action = State::Stay;
	Timer stayTimer = 1.0f;
	static constexpr int nJumps = 3;
	int curJumps = 0;
	static constexpr float jumpSpeed = 300.0f;
	static constexpr float jumpRange = 180.0f;
	static constexpr float shotgunRange = 190.0f;
	static constexpr float bulletSpeed = 250.0f;
	static constexpr float bulletSpacing = chili::pi / 2.0f;
	bool explLastFrame = false;
};
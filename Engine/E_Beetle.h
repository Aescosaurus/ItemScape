#pragma once

#include "E_EnemyBase.h"
#include "Anim.h"
#include "Codex.h"
#include "Timer.h"
#include "Collider.h"
#include <vector>
#include "Bullet.h"
#include <memory>

class Beetle
	:
	public EnemyBase
{
private:
	enum class State
	{
		Moving,
		Firing,
		WindingDown
	};
public:
	Beetle( const Vec2& pos,const TileMap& map,
		std::vector<std::unique_ptr<Bullet>>& bulletVec );

	void Update( const Vec2& playerPos,float dt ) override;
	void Draw( Graphics& gfx ) const override;
private:
	void ResetTargeting();

	Vec2 FindTarget() const;
	Vec2 GetCenter() const;
private:
	static constexpr Vei2 size = { 32,32 };
	static constexpr int maxHP = 10;
	const Surface* pSprSheet = Codex<Surface>::RetrieveSurf( "Images/BeetleAnim.bmp",{ 4,4 } );
	Anim walking; // first row of anim
	Anim firing; // second row of anim
	Anim windDown; // third row
	State myAction = State::Moving;
	Timer shotTimer = 2.4f;
	Collider coll;
	static constexpr float moveTolerance = 145.0f;
	Vec2 target;
	Vec2 lastTarget = pos;
	Vec2 vel;
	static constexpr float speed = 85.1f;
	Vec2 shotTarget;
	std::vector<std::unique_ptr<Bullet>>* pBulletVec;
	const TileMap* map;
	static constexpr float myBulletSpeed = 215.5f;
};
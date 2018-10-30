#pragma once

#include "E__EnemyBase.h"
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
		WindingDown,
		Exploding
	};
public:
	Beetle( const Vec2& pos,const TileMap& map,
		std::vector<std::unique_ptr<Bullet>>& bulletVec );

	void Update( const Vec2& playerPos,float dt ) override;
	void Draw( Graphics& gfx ) const override;

	void Attack( int damage,const Vec2& loc ) override;
private:
	void ResetTargeting();
	void ResetTimer();

	Vec2 FindTarget() const;
	Vec2 GetCenter() const;
private:
	static constexpr Vei2 size = { 32,32 };
	static constexpr int maxHP = 7;
	const Surface* pSprSheet = Codex<Surface>::RetrieveSurf( "Images/BeetleAnim.bmp",{ 4,4 } );
	Anim walking; // first row of anim
	Anim firing; // second row of anim
	Anim windDown; // third row
	Anim explAnim; // 4th row.
	State myAction = State::Moving;
	static constexpr Vec2 timerMinMax = { 1.8f,3.4f };
	Timer shotTimer = 2.4f;
	static constexpr float moveTolerance = 145.0f;
	Vec2 target;
	Vec2 lastTarget = pos;
	Vec2 vel;
	static constexpr float speed = 85.1f;
	Vec2 shotTarget;
	std::vector<std::unique_ptr<Bullet>>* pBulletVec;
	const TileMap* map;
	static constexpr float myBulletSpeed = 215.5f;
	bool deadDead = false;
	bool explDrawDir;
};
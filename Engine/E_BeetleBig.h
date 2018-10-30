#pragma once

#include "E__EnemyBase.h"
#include <vector>
#include "Bullet.h"
#include "Codex.h"
#include "Anim.h"
#include "Timer.h"
#include <memory>
#include "E_Beetle.h"

class BeetleBig
	:
	public EnemyBase
{
private:
	enum class State
	{
		Moving,
		WindingUp,
		WindingDown,
		Exploding
	};
public:
	BeetleBig( const Vec2& pos,const TileMap& map,
		std::vector<std::unique_ptr<Bullet>>& bulletVec,
		std::vector<std::unique_ptr<EnemyBase>>& enemies );

	void Update( const Vec2& playerPos,float dt ) override;
	void Draw( Graphics& gfx ) const override;

	void Attack( int damage,const Vec2& loc ) override;
private:
	void Retarget( const Vec2& theTarget );
	void SpawnBulletCircle();

	Vec2 GetCenter() const;
private:
	static constexpr Vei2 size = { 64,64 };
	static constexpr int myHP = 16;
	std::vector<std::unique_ptr<Bullet>>* pBulletVec;
	std::vector<std::unique_ptr<EnemyBase>>* pEnemyVec;
	const Surface* sprSheet = Codx::Load( "Images/BeetleBigAnim.bmp",{ 4,4 } );
	Anim walking;
	Anim jumping;
	Anim landing;
	Anim explode;
	State curState = State::Moving;
	Vec2 vel = { 0.0f,0.0f };
	Timer moveReset = 3.54f;
	Timer retarget = 0.7f;
	static constexpr float speed = 79.24f;
	static constexpr float bullSpeed = 186.2f;
	bool deadDead = false;
};
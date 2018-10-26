#pragma once

#include "E_EnemyBase.h"
#include <vector>
#include "Bullet.h"
#include "Codex.h"
#include "Anim.h"
#include "Timer.h"

class BeetleBig
	:
	public EnemyBase
{
private:
	enum class State
	{
		Moving,
		WindingUp,
		WindingDown
	};
public:
	BeetleBig( const Vec2& pos,const TileMap& map,
		std::vector<Bullet>& bulletVec );

	void Update( const Vec2& playerPos,float dt ) override;
	void Draw( Graphics& gfx ) const override;
private:
	void Retarget( const Vec2& theTarget );

	Vec2 GetCenter() const;
private:
	static constexpr Vei2 size = { 64,64 };
	static constexpr int myHP = 10;
	std::vector<Bullet>* pBulletVec;
	// TODO: Make the big beetle enemy jump up and land and shoot a circle of bullets when it lands.
	const Surface* sprSheet = Codx::Load( "Images/BeetleBigAnim.bmp",{ 4,4 } );
	Anim walking;
	Anim jumping;
	Anim landing;
	State curState = State::Moving;
	Vec2 vel = { 0.0f,0.0f };
	Timer moveReset = 3.54f;
	Timer retarget = 0.7f;
	static constexpr float speed = 79.24f;
	static constexpr float bullSpeed = 186.2f;
};
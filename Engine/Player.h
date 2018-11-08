#pragma once

#include "Vec2.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "TileMap.h"
#include "Collider.h"
#include "Bullet.h"
#include "Mouse.h"
#include "Timer.h"
#include <memory>

// Works like top down now, but can be modified
//  to use platformer controls.
class Player
{
public:
	Player( const Vec2& pos,const TileMap& map,
		std::vector<std::unique_ptr<Bullet>>& bullets );

	void Update( const Keyboard& kbd,const Mouse& ms,
		float dt );
	void Draw( Graphics& gfx ) const;

	const Vec2& GetPos() const;
	const Vec2& GetVel() const;
	Vec2 GetCenter() const;
private:
	Vec2 pos;
	static constexpr Vei2 size = { 16,16 };
	static constexpr float speed = 171.4f;
	Collider coll;
	std::vector<std::unique_ptr<Bullet>>& myBullets;
	Timer shotTimer = { 0.23f };
	const TileMap& map;
	static constexpr float bulletSpeed = 324.2f;
	Vec2 moveDir = { 0.0f,0.0f };
};
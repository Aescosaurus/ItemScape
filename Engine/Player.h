#pragma once

#include "Vec2.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "TileMap.h"
#include "Collider.h"
#include "Bullet.h"
#include "Mouse.h"
#include "Timer.h"

// Works like top down now, but can be modified
//  to use platformer controls.
class Player
{
public:
	Player( const Vec2& pos,const TileMap& map,
		std::vector<Bullet>& bullets );

	void Update( const Keyboard& kbd,const Mouse& ms,
		float dt );
	void Draw( Graphics& gfx ) const;

	const Vec2& GetPos() const;
	Vec2 GetCenter() const;
private:
	Vec2 pos;
	static constexpr Vei2 size = { 32,32 };
	static constexpr float speed = 171.4f;
	Collider coll;
	std::vector<Bullet>& myBullets;
	Timer shotTimer = { 0.23f };
	const TileMap& map;
	static constexpr float bulletSpeed = 324.2f;
};
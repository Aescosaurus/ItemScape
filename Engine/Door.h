#pragma once

#include "Vec2.h"
#include "Graphics.h"
#include "Surface.h"
#include "Rect.h"
#include "FloorLevel.h"

class Door
{
public:
	enum class Side
	{
		Top,
		Bot,
		Left,
		Right
	};
public:
	Door( Side s,const FloorLevel& floor );

	void Draw( Graphics& gfx ) const;

	void UpdateActivated( const FloorLevel& floor );

	bool IsActivated() const;
	const RectI& GetRect() const;
	Side GetSide() const;
private:
	Vei2 GetPosFromSide( Side s ) const;
	RectI GetRectFromSide( Side s ) const;
private:
	const Vei2 pos;
	const RectI hitbox;
	static constexpr Vei2 dim = { 64,32 };
	static const Surface topSurf;
	static const Surface botSurf;
	static const Surface sideSurf;
	Side mySide;
	bool activated = false;
};
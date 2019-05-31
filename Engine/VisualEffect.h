#pragma once
#include "Graphics.h"
#include "Anim.h"

class VisualEffect
{
public:
	enum class Type
	{
		Lightning = 0,
		Fire,
		Poison,
		Dust,
		Smoke
	};
public:
	VisualEffect( const Vei2& pos,Type t );

	void Update( float dt );
	void Draw( Graphics& gfx ) const;

	bool IsExpl() const;
private:
	static constexpr Vei2 size = { 32,32 };
	Anim anim;
	Vei2 pos;
	static const Surface sprSheet;
	bool expl = false;
};
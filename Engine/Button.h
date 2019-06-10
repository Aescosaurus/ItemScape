#pragma once

#include "Vec2.h"
#include <string>
#include "Mouse.h"
#include "Graphics.h"
#include "Codex.h"

class Button
{
public:
	Button( const Vei2& pos,const std::string& text );

	void Update( const Mouse& mouse );
	void Draw( Graphics& gfx ) const;

	bool IsDown() const;
private:
	CFontPtr font = FontCodex::Fetch( "Fonts/LuckyPixel6x9.bmp" );
	const Vei2 pos;
	const Vei2 size;
	const std::string text;
	static constexpr Vei2 padding = { 5,5 };
	bool hovering = false;
	bool canClick = false;
	bool clicking = false;
};
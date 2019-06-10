#include "Button.h"

Button::Button( const Vei2& pos,const std::string& text )
	:
	pos( pos ),
	text( text ),
	size( int( text.length() ) * font->GetGlyphSize().x +
		padding.x * 2,font->GetGlyphSize().y + padding.y * 2 )
{}

void Button::Update( const Mouse& mouse )
{
	hovering = ( Rect{ pos,pos + size }
		.ContainsPoint( mouse.GetPos() ) );

	if( mouse.LeftIsPressed() && hovering && canClick )
	{
		clicking = true;
	}
	else clicking = false;

	if( !mouse.LeftIsPressed() ) canClick = true;
	else canClick = false;
}

void Button::Draw( Graphics& gfx ) const
{
	gfx.DrawRect( pos.x,pos.y,size.x,size.y,
		hovering ? Colors::LightGray : Colors::Gray );

	font->DrawText( text,pos + padding,Colors::White,gfx );
}

bool Button::IsDown() const
{
	return( clicking );
}

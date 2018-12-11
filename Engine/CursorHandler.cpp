#include "CursorHandler.h"
#include "SpriteEffect.h"

CursorHandler::CursorHandler( MainWindow& wnd )
	:
	wnd( wnd )
{}

void CursorHandler::DrawCursor( const Vei2& pos,
	GameState state,Graphics& gfx ) const
{
	switch( state )
	{
	case GameState::Menu:
		break;
	case GameState::Gameplay:
		gfx.DrawSprite( pos.x - crosshair.GetWidth() / 2,
			pos.y - crosshair.GetHeight() / 2,crosshair,
			SpriteEffect::Chroma{ Colors::Magenta } );
		break;
	}
}

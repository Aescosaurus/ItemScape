#pragma once

#include "Vec2.h"
#include "MainWindow.h"
#include "Graphics.h"
#include "GameState.h"
#include "Surface.h"

class CursorHandler
{
public:
	CursorHandler( MainWindow& wnd );

	void DrawCursor( const Vei2& pos,
		GameState state,Graphics& gfx ) const;
private:
	MainWindow& wnd;
	const Surface crosshair = "Images/Crosshair.bmp";
};
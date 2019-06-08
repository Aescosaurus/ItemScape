/******************************************************************************************
 *	Chili DirectX Framework Version 16.07.20											  *
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	campaign( wnd,gfx ),
	cursorHand( wnd )
{
	wnd.Maximize();
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	if( wnd.kbd.KeyIsPressed( VK_CONTROL ) &&
		wnd.kbd.KeyIsPressed( 'F' ) ) wnd.Maximize();
	if( wnd.kbd.KeyIsPressed( VK_ESCAPE ) ) wnd.Minimize();
	if( wnd.kbd.KeyIsPressed( VK_CONTROL ) &&
		wnd.kbd.KeyIsPressed( 'W' ) ) wnd.Kill();

	switch( state )
	{
	case GameState::Menu:
		break;
	case GameState::Gameplay:
		campaign.Update();
		break;
	}
}

void Game::ComposeFrame()
{
	switch( state )
	{
	case GameState::Menu:
		break;
	case GameState::Gameplay:
		campaign.Draw();
		break;
	}

	cursorHand.DrawCursor( wnd.mouse.GetPos(),state,gfx );
}

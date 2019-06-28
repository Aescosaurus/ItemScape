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

#include "SaveLoader.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	cursorHand( wnd ),
	campaign( wnd,gfx ),
	menu( wnd,campaign.GetFloor(),campaign.GetInv(),state,
		saveSlot )
{
	wnd.Maximize();
}

void Game::Go()
{
	UpdateModel();
	gfx.BeginFrame();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	if( wnd.kbd.KeyIsPressed( VK_CONTROL ) &&
		wnd.kbd.KeyIsPressed( 'F' ) ) wnd.Maximize();
	if( wnd.kbd.KeyIsPressed( VK_ESCAPE ) ) wnd.Minimize();
	if( wnd.kbd.KeyIsPressed( VK_CONTROL ) &&
		wnd.kbd.KeyIsPressed( 'W' ) )
	{
		if( saveSlot != -1 )
		{
			SaveLoader::Save( SaveLoaderInfo{
				campaign.GetFloor(),campaign.GetInv(),gfx,campaign.GetMap() },
				saveSlot );
		}
		wnd.Kill();
	}

	switch( state )
	{
	case GameState::Menu:
		if( menu.Update( campaign.GenerateInvEvtInfo(),wnd.mouse ) )
		{
			state = GameState::Gameplay;
			campaign.LoadNextLevel();
			campaign.SetUpLevelStart();
		}
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
		menu.Draw( gfx );
		break;
	case GameState::Gameplay:
		campaign.Draw();
		break;
	}

	cursorHand.DrawCursor( wnd.mouse.GetPos(),state,gfx );
}

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
#include "FrameTimer.h"
#include "Utils.h"
#include <functional>

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	map( "Maps/Map1.lvl" ),
	guy( { 150.0f,150.0f },map,playerBullets )
{
	enemies.emplace_back( new BeetleBig{ Vec2{ 50,50 },map,enemyBullets } );
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
	auto dt = FrameTimer::Mark();
	if( dt > 0.5f ) dt = 0.0f;

	guy.Update( wnd.kbd,wnd.mouse,dt );

	for( auto& e : enemies ) e->Update( guy.GetPos(),dt );

	for( auto& b : playerBullets ) b.Update( dt );
	for( auto& eb : enemyBullets ) eb.Update( dt );

	chili::remove_erase_if( playerBullets,std::mem_fn( &Bullet::IsDead ) );
	chili::remove_erase_if( enemyBullets,std::mem_fn( &Bullet::IsDead ) );
}

void Game::ComposeFrame()
{
	map.Draw( gfx );
	for( const auto& e : enemies ) e->Draw( gfx );
	for( const auto& b : playerBullets ) b.Draw( gfx );
	for( const auto& eb : enemyBullets ) eb.Draw( gfx );
	guy.Draw( gfx );
}

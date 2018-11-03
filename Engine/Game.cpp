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
#include "EnemyType.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	guy( { 150.0f,150.0f },map,playerBullets )
{
	LoadNextLevel();
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
	if( dt > 1.0f / 10.0f ) dt = 0.0f;

	guy.Update( wnd.kbd,wnd.mouse,dt );

	for( auto& b : playerBullets ) b->Update( dt );
	for( auto& eb : enemyBullets ) eb->Update( dt );

	// Update all enemies with a regular for loop because
	//  Enemy::Update might invalidate the iterator.
	for( int i = 0; i < int( enemies.size() ); ++i )
	{
		const EnemyUpdateInfo euInfo = { guy.GetPos(),guy.GetVel() };
		enemies[i]->Update( euInfo,dt );

		// Update might add new enemies and invalidate the
		//  reference, so just init it here.
		const auto& e = enemies[i];

		const auto enemyRect = e->GetRect();
		for( auto& b : playerBullets )
		{
			if( b->GetRect().IsOverlappingWith( enemyRect ) )
			{
				e->Attack( 1,b->GetRect().GetCenter() );
				b->Attack( 1 );
			}
		}
	}

	// chili::remove_erase_if( enemies,std::mem_fn( &EnemyBase::IsDead ) );
	chili::remove_erase_if( playerBullets,std::mem_fn( &Bullet::IsDead ) );
	chili::remove_erase_if( enemyBullets,std::mem_fn( &Bullet::IsDead ) );
}

void Game::ComposeFrame()
{
	map.Draw( gfx );
	for( const auto& e : enemies ) e->Draw( gfx );
	for( const auto& b : playerBullets ) b->Draw( gfx );
	for( const auto& eb : enemyBullets ) eb->Draw( gfx );
	guy.Draw( gfx );
}

void Game::LoadNextLevel()
{
	// Get rid of exploded enemies and existing bullets.
	enemies.clear();
	playerBullets.clear();
	enemyBullets.clear();

	// Make up name for the level after this one.
	static constexpr auto first = "Levels/Level";
	static constexpr auto second = ".lvl";
	const std::string nextLevelName = first +
		std::to_string( curLevel++ ) + second;

	map.LoadFile( nextLevelName );

	// Get everything in the map that isn't a wall or floor.
	const auto terms = map.FindSpecialTerms( nextLevelName );

	// Load big beetles into vec.
	for( const auto& t : terms )
	{
		switch( t.type )
		{
		case char( EnemyType::BigBeetle ) :
			enemies.emplace_back( std::make_unique<BeetleBig>(
				t.pos,map,enemyBullets,enemies ) );
			break;
		case char( EnemyType::Firebug ) :
			enemies.emplace_back( std::make_unique<Firebug>(
				t.pos,map,enemyBullets ) );
			break;
		}
	}
}
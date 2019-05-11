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
#include "BeetleBig.h"
#include "Firebug.h"
#include "Skorp.h"
#include "SpiderMole.h"
#include "RynoChaser.h"
#include "Slizard.h"
#include "HealthCharge.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	guy( Vec2( Graphics::GetScreenRect().GetCenter() ),map,playerBullets ),
	cursorHand( wnd )
{
	wnd.Maximize();

	LoadNextLevel();
	doors.emplace_back( Door{ Door::Side::Top,floor } );
	doors.emplace_back( Door{ Door::Side::Bot,floor } );
	doors.emplace_back( Door{ Door::Side::Left,floor } );
	doors.emplace_back( Door{ Door::Side::Right,floor } );

	playerInv.AddItem( new HealthCharge );
	playerInv.AddItem( new HealthCharge );
	playerInv.AddItem( new HealthCharge );
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
#if !NDEBUG
	if( wnd.kbd.KeyIsPressed( VK_RETURN ) )
	{
		for( const auto& e : enemies )
		{
			e->Attack( 9999,{ 0.0f,0.0f } );
		}
	}
#endif

	if( wnd.kbd.KeyIsPressed( VK_CONTROL ) &&
		wnd.kbd.KeyIsPressed( 'F' ) ) wnd.Maximize();
	if( wnd.kbd.KeyIsPressed( VK_ESCAPE ) ) wnd.Minimize();
	if( wnd.kbd.KeyIsPressed( VK_CONTROL ) &&
		wnd.kbd.KeyIsPressed( 'W' ) ) wnd.Kill();

	auto dt = FrameTimer::Mark();
	if( dt > 1.0f / 10.0f ) dt = 0.0f;

	guy.Update( wnd.kbd,wnd.mouse,dt );

	playerInv.Update( wnd.kbd,wnd.mouse );

	for( auto& b : playerBullets ) b->Update( dt );
	for( int i = 0; i < enemyBullets.size(); ++i )
	{
		auto& eb = enemyBullets[i];

		eb->Update( dt );

		if( eb->GetRect().IsOverlappingWith( guy.GetRect() ) )
		{
			if( playerInv.FindItem( "Health Charge" ) != nullptr )
			{
				playerInv.OnPlayerHit( InventoryEventInfo{
					guy,enemyBullets,visualEffects } );
			}
			else
			{
				// You lose!!
			}
		}
	}

	// Update all enemies with a regular for loop because
	//  Enemy::Update might invalidate the iterator.
	bool enemyExploded = false;
	const EnemyUpdateInfo euInfo = { guy.GetPos(),
		guy.GetVel() };
	for( int i = 0; i < int( enemies.size() ); ++i )
	{
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

				if( e->IsExpl() ) enemyExploded = true;
			}
		}
	}

	// Sort the list so enemies that have exploded get
	//  drawn behind ones that haven't.
	if( enemyExploded )
	{
		std::sort( enemies.begin(),enemies.end(),
			[]( const std::unique_ptr<EnemyBase>& first,
				const std::unique_ptr<EnemyBase>& second )
		{
			return( first->IsExpl() && !second->IsExpl() );
		} );
	}

	for( auto& vEff : visualEffects ) vEff.Update( dt );

	chili::remove_erase_if( playerBullets,std::mem_fn( &Bullet::IsExpl ) );
	chili::remove_erase_if( enemyBullets,std::mem_fn( &Bullet::IsExpl ) );
	chili::remove_erase_if( visualEffects,std::mem_fn( &VisualEffect::IsExpl ) );

	// Check if you're touching a door.
	if( IsLevelOver() )
	{
		const auto& guyRect = guy.GetRect();
		for( const auto& d : doors )
		{
			if( guyRect.IsOverlappingWith( d.GetRect() ) &&
				d.IsActivated() )
			{
				guy.MoveTo( d.GetPlayerSpawnPos( guy.GetPos() ) );
				floor.MoveRoom( FloorLevel::Dir( d.GetSide() ) );
				LoadNextLevel();
				for( auto& d1 : doors ) d1.UpdateActivated( floor );
				break;
			}
		}
	}
}

void Game::ComposeFrame()
{
	// Bottom of drawing order.
	map.DrawFloor( gfx );
	for( const auto& d : doors ) d.Draw( gfx );
	for( const auto& e : enemies ) e->Draw( gfx );
	for( const auto& eb : enemyBullets ) eb->Draw( gfx );
	for( const auto& b : playerBullets ) b->Draw( gfx );
	for( const auto& eff : visualEffects ) eff.Draw( gfx );
	map.Draw( gfx );
	guy.Draw( gfx );
	floor.DrawOverlay( gfx );
	playerInv.Draw( gfx );
	// Top of drawing order.

	cursorHand.DrawCursor( wnd.mouse.GetPos(),state,gfx );
}

void Game::LoadNextLevel()
{
	// Get rid of exploded enemies and any leftover bullets.
	enemies.clear();
	playerBullets.clear();
	enemyBullets.clear();

	map.LoadFile( floor.GetLevelName() );

	// Get everything in the map that isn't a wall or floor.
	const auto terms = map.FindSpecialTerms( floor.GetLevelName() );

	// Load enemies into vec.
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
		case char( EnemyType::Skorp ) :
			enemies.emplace_back( std::make_unique<Skorp>(
				t.pos,map,enemyBullets ) );
			break;
		case char( EnemyType::SpiderMole ) :
			enemies.emplace_back( std::make_unique<SpiderMole>(
				t.pos,map,enemyBullets ) );
			break;
		case char( EnemyType::RynoChaser ) :
			enemies.emplace_back( std::make_unique<RynoChaser>(
				t.pos,map,enemyBullets ) );
			break;
		case char( EnemyType::Slizard ) :
			enemies.emplace_back( std::make_unique<Slizard>(
				t.pos,map,enemyBullets ) );
			break;
		}
	}

	if( floor.CurRoomAlreadyCompleted() )
	{
		enemies.clear();
	}
}

bool Game::IsLevelOver() const
{
	for( const auto& e : enemies )
	{
		if( !e->IsExpl() ) return( false );
	}
	return( true );
}

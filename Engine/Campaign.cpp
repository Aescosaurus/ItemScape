#include "Campaign.h"

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
#include "WingedBugMidboss.h"
#include "WormBugBoss.h"

#include "PickupManager.h"

Campaign::Campaign( MainWindow& wnd,Graphics& gfx )
	:
	kbd( wnd.kbd ),
	mouse( wnd.mouse ),
	gfx( gfx ),
	guy( Vec2( Graphics::GetScreenRect().GetCenter() ),map,
		playerBullets,visualEffects )
{
	PickupManager::Initialize();

	LoadNextLevel();
	doors.emplace_back( Door{ Door::Side::Top,floor } );
	doors.emplace_back( Door{ Door::Side::Bot,floor } );
	doors.emplace_back( Door{ Door::Side::Left,floor } );
	doors.emplace_back( Door{ Door::Side::Right,floor } );

	// GotoNextFloor();
}

Campaign::~Campaign()
{
	for( auto* p : pickups )
	{
		delete p;
	}

	PickupManager::Free();
}

void Campaign::Update()
{
#if !NDEBUG
	if( kbd.KeyIsPressed( VK_RETURN ) )
	{
		// for( const auto& e : enemies )
		// {
		// 	e->Attack( 9999,{ 0.0f,0.0f } );
		// }
		ClearEnemies();
	}
#endif

	auto dt = FrameTimer::Mark();
	if( dt > 1.0f / 10.0f ) dt = 0.0f;

	if( frozenTime > 0.0f )
	{
		frozenTime -= dt;
		if( frozenTime < 0.0f ) frozenTime = 0.0f;
		return;
	}

	if( playerInv.IsOpen() ) dt = 0.0f;

	playerInv.Update( kbd,mouse,GenerateInvEvtInfo( dt ) );

	guy.Update( kbd,mouse,dt );

	if( guy.JustShot() ) playerInv.OnPlayerShoot( GenerateInvEvtInfo( dt ) );

	for( int i = 0; i < int( playerBullets.size() ); ++i )
	{
		auto& b = playerBullets[i];
		b->Update( GenerateBulletEvtInfo( dt ) );
	}
	for( int i = 0; i < int( enemyBullets.size() ); ++i )
	{
		auto& eb = enemyBullets[i];

		eb->Update( GenerateBulletEvtInfo( dt ) );

		if( eb->GetRect().IsOverlappingWith( guy.GetRect() ) )
		{
			frozenTime += freezeFrameDuration * 3.0f;
			eb->Attack( 1,&visualEffects );
			if( !guy.IsInvul() )
			{
				if( playerInv.FindItem( "Health Charge" ) != nullptr )
				{
					playerInv.OnPlayerHit( GenerateInvEvtInfo( dt ) );
				}
				else
				{
					// You lose!!
				}
			}
		}
	}

	// Update all enemies with a regular for loop because
	//  Enemy::Update might invalidate the iterator.
	bool enemyExploded = false;
	const EnemyUpdateInfo euInfo = { guy.GetPos(),
		guy.GetVel(),doors };
	for( int i = 0; i < int( enemies.size() ); ++i )
	{
		enemies[i]->Update( euInfo,dt );

		// Update might add new enemies and invalidate the
		//  reference, so just init it here.
		const auto& e = enemies[i];
		const auto enemyRect = e->GetRect();

		// for( auto& b : playerBullets )
		for( int i = 0; i < int( playerBullets.size() ); ++i )
		{
			auto& b = playerBullets[i];

			if( b->GetRect().IsOverlappingWith( enemyRect ) )
			{
				frozenTime += freezeFrameDuration * b->GetDamage();
				e->Attack( b->GetDamage(),
					b->GetRect().GetCenter() );
				b->Attack( 1,&visualEffects );

				playerInv.OnEnemyHit( GenerateInvEvtInfo( dt,e.get(),b.get() ) );

				if( e->IsExpl() )
				{
					enemyExploded = true;
					playerInv.OnEnemyExplode( GenerateInvEvtInfo( dt,e.get(),b.get() ) );
				}

				if( IsLevelOver() )
				{
					pickupPos = e->GetPos() + Vec2{
						Random::RangeF( 0.0f,e->GetRect()
						.GetWidth() / 2.0f ),
						Random::RangeF( 0.0f,e->GetRect()
							.GetHeight() / 2.0f ) };

					// spawnedEndOfLevelItem = false;
				}
			}
		}
	}

	// Sort the list so enemies that have exploded get
	//  drawn behind ones that haven't.
	// if( enemyExploded )
	{
		std::sort( enemies.begin(),enemies.end(),
			[]( const std::unique_ptr<EnemyBase>& first,
				const std::unique_ptr<EnemyBase>& second )
		{
			return( first->IsExpl() && !second->IsExpl() );
		} );
	}

	for( auto& vEff : visualEffects ) vEff.Update( dt );

	// Pick up pickups.
	for( auto it = pickups.begin(); it != pickups.end(); ++it )
	{
		if( ( *it )->GetRect().IsOverlappingWith( guy.GetRect() ) )
		{
			const auto tier = ( *it )->GetTier();
			playerInv.AddItem( *it,GenerateInvEvtInfo( dt ) );
			pickups.erase( it );
			if( tier == 2 )
			{
				GotoNextFloor();
				// return;
			}
			break;
		}
	}

	chili::remove_erase_if( playerBullets,std::mem_fn( &Bullet::IsExpl ) );
	chili::remove_erase_if( enemyBullets,std::mem_fn( &Bullet::IsExpl ) );
	chili::remove_erase_if( visualEffects,std::mem_fn( &VisualEffect::IsExpl ) );

	if( IsLevelOver() )
	{
		// Spawn item after last enemy explodes.
		if( !spawnedEndOfLevelItem )
		{
			bool spawnT2Item = false;
			for( auto& e : enemies )
			{
				if( e->IsBoss() )
				{
					spawnT2Item = true;
					break;
				}
			}
			if( !spawnT2Item )
			{
				pickups.emplace_back( PickupManager
					::RandT1Pickup() );
				pickups.back()->SetPos( pickupPos );
			}
			else // Spawn a tier 2 item.
			{
				const int nItems = int( playerInv.GetItemVec().size() );
				for( int i = 0; i < 3; ++i )
				{
					pickups.emplace_back( PickupManager
						::RandT2Pickup() );
					// T2 items are free if you have less than 3 items.
					if( nItems > 2 )
					{
						const int randAmount = Random::RangeI( 2,
							std::min( 5,nItems / 2 ) );
						for( int i = 0; i < randAmount; ++i )
						{
							pickups.back()->AddRemoveIndex(
								Random::RangeI( 1,nItems - 1 ) );
						}
					}
					pickups.back()->SetPos( Vei2{
						Random::RangeI( 64,Graphics::ScreenWidth - 64 ),
						Random::RangeI( 64,Graphics::ScreenHeight - 96 ) } );
				}
			}
			visualEffects.emplace_back( VisualEffect{
				pickupPos,VisualEffect::Type
				::Lightning } );

			spawnedEndOfLevelItem = true;
		}

		// Check if you're touching a door.
		const auto& guyRect = guy.GetRect();
		for( const auto& d : doors )
		{
			if( guyRect.IsOverlappingWith( d.GetRect() ) &&
				d.IsActivated() )
			{
				guy.MoveTo( d.GetPlayerSpawnPos( guy.GetPos() ) );
				floor.MoveRoom( FloorLevel::Dir( d.GetSide() ) );
				LoadNextLevel();
				break;
			}
		}
	}
}

void Campaign::Draw()
{
	// Bottom of drawing order.
	map.DrawFloor( gfx );
	for( const auto& d : doors ) d.Draw( gfx );
	for( const auto& e : enemies ) e->Draw( gfx );
	for( const auto& eb : enemyBullets ) eb->Draw( gfx );
	for( const auto& b : playerBullets ) b->Draw( gfx );
	for( const auto* pup : pickups ) pup->Draw( gfx,&playerInv );
	map.Draw( gfx );
	for( const auto& eff : visualEffects ) eff.Draw( gfx );
	guy.Draw( gfx );
	playerInv.OnDraw( GenerateInvEvtInfo() );
	floor.DrawOverlay( gfx ); // Draw minimap.
	playerInv.Draw( gfx );
	// Top of drawing order.
}

FloorLevel& Campaign::GetFloor()
{
	return( floor );
}

Inventory& Campaign::GetInv()
{
	return( playerInv );
}

void Campaign::LoadNextLevel()
{
	// Get rid of exploded enemies and any leftover bullets.
	enemies.clear();
	playerBullets.clear();
	enemyBullets.clear();
	visualEffects.clear();
	for( auto* pickup : pickups )
	{
		delete pickup;
	}
	pickups.clear();

	pickupPos = Graphics::GetScreenRect().GetCenter();

	map.LoadFile( floor.GetLevelName() );
	for( auto& d1 : doors ) d1.UpdateActivated( floor );

	// Get everything in the map that isn't a wall or floor.
	const auto terms = map.FindSpecialTerms( floor.GetLevelName() );

	// Load enemies into vec.
	for( const auto& t : terms )
	{
		switch( t.type )
		{
		case ( char )EnemyType::BigBeetle:
			enemies.emplace_back( std::make_unique<BeetleBig>(
				t.pos,map,enemyBullets,enemies ) );
			break;
		case ( char )EnemyType::Firebug:
			enemies.emplace_back( std::make_unique<Firebug>(
				t.pos,map,enemyBullets ) );
			break;
		case ( char )EnemyType::Skorp:
			enemies.emplace_back( std::make_unique<Skorp>(
				t.pos,map,enemyBullets ) );
			break;
		case ( char )EnemyType::SpiderMole:
			enemies.emplace_back( std::make_unique<SpiderMole>(
				t.pos,map,enemyBullets ) );
			break;
		case ( char )EnemyType::RynoChaser:
			enemies.emplace_back( std::make_unique<RynoChaser>(
				t.pos,map,enemyBullets ) );
			break;
		case ( char )EnemyType::Slizard:
			enemies.emplace_back( std::make_unique<Slizard>(
				t.pos,map,enemyBullets ) );
			break;
		case ( char )EnemyType::WingedBugMidboss:
			enemies.emplace_back( std::make_unique<WingedBugMidboss>(
				t.pos,map,enemyBullets ) );
			break;
		case ( char )EnemyType::WormBugBoss:
			enemies.emplace_back( std::make_unique<WormBugBoss>(
				t.pos,map,enemyBullets,enemies ) );
			break;
		}
	}

	playerInv.ToggleItemSwapping( true );
	for( const auto& enemy : enemies )
	{
		if( enemy->IsBoss() )
		{
			playerInv.ToggleItemSwapping( false );
		}
	}

	if( floor.CurRoomAlreadyCompleted() )
	{
		enemies.clear();
		spawnedEndOfLevelItem = true;
	}
	else
	{
		spawnedEndOfLevelItem = false;
	}

	playerInv.OnRoomStart( GenerateInvEvtInfo() );
}

void Campaign::ClearEnemies()
{
	enemies.clear();
	spawnedEndOfLevelItem = true;
}

bool Campaign::IsLevelOver() const
{
	for( const auto& e : enemies )
	{
		if( !e->IsExpl() ) return( false );
	}
	return( true );
}

InventoryEventInfo Campaign::GenerateInvEvtInfo( float dt,
	EnemyBase* hitEnemy,Bullet* curBullet )
{
	return( InventoryEventInfo{ guy,enemyBullets,
		visualEffects,playerBullets,enemies,pickups,dt,
		mouse,map,playerInv.GetItemVec(),gfx,
		hitEnemy,curBullet } );
}

BulletUpdateInfo Campaign::GenerateBulletEvtInfo( float dt )
{
	return( BulletUpdateInfo{ enemies,dt,guy,visualEffects,
		playerBullets } );
}

void Campaign::GotoNextFloor()
{
	floor.AdvanceFloor();
	map.AdvanceFloor();
	LoadNextLevel();
}

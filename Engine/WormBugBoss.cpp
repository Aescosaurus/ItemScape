#include "WormBugBoss.h"
#include "Utils.h"
#include "BeetleBig.h"
#include "Firebug.h"
#include "RynoChaser.h"
#include "Skorp.h"
#include "Slizard.h"
#include "SpiderMole.h"

WormBugBoss::WormBugBoss( const Vec2& pos,const TileMap& map,
	std::vector<std::unique_ptr<Bullet>>& bullets,
	std::vector<std::unique_ptr<EnemyBase>>& enemies )
	:
	EnemyBase( pos,size,health,map ),
	pBulletVec( &bullets ),
	pEnemyVec( &enemies ),
	wandering( 0,0,size.x,size.y,4,*surfSheet,0.2f ),
	diggingDown( 0,size.y,size.x,size.y,4,*surfSheet,0.2f ),
	diggingUp( 0,size.y * 2,size.x,size.y,4,*surfSheet,0.2f ),
	exploding( 0,size.y * 3,size.x,size.y,4,*surfSheet,0.2f )
{
	EnemyBase::ResetTargeting( moveRange,moveSpeed );
}

void WormBugBoss::Update( const EnemyUpdateInfo& info,float dt )
{
	EnemyBase::Update( info,dt );

	switch( action )
	{
	case State::Wander:
		wandering.Update( dt );
		wanderTimer.Update( dt );
		EnemyBase::Wander( moveRange,moveSpeed,dt );

		if( reachedTarget )
		{
			const int nShots = 8;
			for( int i = 0; i < nShots; ++i )
			{
				const auto vel = Vec2{ 0.0f,-1.0f }
					.Deviate( ( chili::pi * 2.0f ) *
					( float( i ) / float( nShots ) ) );

				pBulletVec->emplace_back( std::make_unique<
					Bullet>( GetRect().GetCenter(),
					GetRect().GetCenter() + vel,*map,
					Bullet::Team::WormBoss,bulletSpeed,
					Bullet::Size::Medium ) );
			}

			if( Random::RangeI( 0,100 ) < bugSpawnChance )
			{
				EnemyBase* spawnedEnemy = nullptr;
				const int rngOutcome = Random::RangeI( 0,5 );
				switch( rngOutcome )
				{
				case 0:
					spawnedEnemy = new BeetleBig{ pos,*map,
						*pBulletVec,*pEnemyVec };
					break;
				case 1:
					spawnedEnemy = new Firebug{ pos,*map,*pBulletVec };
					break;
				case 2:
					spawnedEnemy = new RynoChaser{ pos,*map,
						*pBulletVec };
					break;
				case 3:
					spawnedEnemy = new Skorp{ pos,*map,
						*pBulletVec };
					break;
				case 4:
					spawnedEnemy = new Slizard{ pos,*map,
						*pBulletVec };
					break;
				case 5:
					spawnedEnemy = new SpiderMole{ pos,*map,
						*pBulletVec };
					break;
				default:
					assert( false );
				}

				pEnemyVec->emplace_back( spawnedEnemy );
			}

			EnemyBase::ResetTargeting( moveRange,moveSpeed );
		}

		if( wanderTimer.IsDone() && wandering.IsFinished() )
		{
			wanderTimer.Reset();
			wandering.Reset();
			EnemyBase::ResetTargeting( moveRange,moveSpeed );
			action = State::DigDown;
		}
		break;
	case State::DigDown:
		diggingDown.Update( dt );
		if( diggingDown.IsFinished() )
		{
			diggingDown.Reset();
			target = info.playerPos;
			vel = ( target - pos ).GetNormalized() * tunnelSpeed;
			action = State::Tunnel;
		}
		break;
	case State::Tunnel:
	{
		coll.MoveTo( pos );
		const auto testMove = vel * dt;
		const auto validMove = coll.GetValidMove( pos,testMove );
		pos += validMove;
		coll.MoveTo( Vec2{ -9999.0f,-9999.0f } );
		if( coll.GetRect().ContainsPoint( target ) ||
			validMove.z )
		{
			coll.MoveTo( pos );
			action = State::DigUp;
		}
		break;
	}
	case State::DigUp:
		diggingUp.Update( dt );
		if( diggingUp.IsFinished() )
		{
			diggingUp.Reset();
			EnemyBase::ResetTargeting( moveRange,moveSpeed );
			action = State::Wander;
		}
		break;
	case State::Explode:
		if( !exploding.IsFinished() ) exploding.Update( dt );
		if( exploding.IsFinished() ) exploding.SetFrame( 3 );
		break;
	}
}

void WormBugBoss::Draw( Graphics& gfx ) const
{
	switch( action )
	{
	case State::Wander:
		if( IsFlashing() ) wandering.Draw( Vei2( pos ),gfx,FlashCol(),vel.x < 0.0f );
		else wandering.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		break;
	case State::DigDown:
		if( IsFlashing() ) diggingDown.Draw( Vei2( pos ),gfx,FlashCol(),vel.x < 0.0f );
		else diggingDown.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		break;
	case State::Tunnel:
		// Invisible while underground.
		break;
	case State::DigUp:
		if( IsFlashing() ) diggingUp.Draw( Vei2( pos ),gfx,FlashCol(),vel.x < 0.0f );
		else diggingUp.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		break;
	case State::Explode:
		exploding.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		break;
	}

	// gfx.DrawHitbox( coll.GetRect(),Colors::Green );
	// gfx.DrawLine( pos,target,Colors::Cyan );
}

void WormBugBoss::Attack( int damage,const Vec2& loc )
{
	if( action != State::Tunnel )
	{
		EnemyBase::Attack( damage,loc );

		if( IsExpl() )
		{
			action = State::Explode;
			coll.MoveTo( Vec2{ -9999.0f,-9999.0f } );
		}
	}
}

bool WormBugBoss::IsBoss() const
{
	return( true );
}

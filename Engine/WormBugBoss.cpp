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
			}

			pEnemyVec->emplace_back( spawnedEnemy );

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
		break;
	case State::Tunnel:
		break;
	case State::DigUp:
		break;
	case State::Explode:
		break;
	}
}

void WormBugBoss::Draw( Graphics& gfx ) const
{
	switch( action )
	{
	case State::Wander:
		break;
	case State::DigDown:
		break;
	case State::Tunnel:
		break;
	case State::DigUp:
		break;
	case State::Explode:
		break;
	}
}

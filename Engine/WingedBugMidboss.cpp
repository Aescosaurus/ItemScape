#include "WingedBugMidboss.h"

WingedBugMidboss::WingedBugMidboss( const Vec2& pos,const TileMap& map,
	std::vector<std::unique_ptr<Bullet>>& bullets )
	:
	EnemyBase( pos,size,health,map ),
	pBulletVec( &bullets ),
	floating( 0,0,size.x,size.y,4,*surfSheet,0.2f ),
	buildingUp( 0,size.y,size.x,size.y,4,*surfSheet,0.2f ),
	charging( 0,size.y * 2,size.x,size.y,4,*surfSheet,0.2f ),
	exploding( 0,size.y * 3,size.x,size.y,4,*surfSheet,0.2f )
{
	EnemyBase::ResetTargeting( jumpRange,jumpSpeed );
}

void WingedBugMidboss::Update( const EnemyUpdateInfo& info,float dt )
{
	EnemyBase::Update( info,dt );

	switch( action )
	{
	case State::Stay:
		floating.Update( dt );
		stayTimer.Update( dt );

		if( stayTimer.IsDone() )
		{
			stayTimer.Reset();
			EnemyBase::ResetTargeting( jumpRange,jumpSpeed );
			action = State::Wander;
		}
		break;
	case State::Wander:
		buildingUp.SetFrame( 3 );
		if( curJumps < nJumps )
		{
			EnemyBase::Wander( jumpRange,jumpSpeed,dt );

			if( reachedTarget )
			{
				if( curJumps == nJumps / 2 )
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
							Bullet::Team::WingedBug,bulletSpeed,
							Bullet::Size::Medium ) );
					}
				}
				++curJumps;
				action = State::Stay;
			}
		}
		else
		{
			EnemyBase::ResetTargeting( jumpRange,jumpSpeed );
			curJumps = 0;
			buildingUp.Reset();
			action = State::BuildUp;
		}
		break;
	case State::BuildUp:
		buildingUp.Update( dt );

		if( buildingUp.IsFinished() )
		{
			action = State::ChargeAttack;
		}
		break;
	case State::ChargeAttack:
		target = info.playerPos;
		vel = ( target - GetRect().GetCenter() )
			.GetNormalized() * jumpSpeed;
		AttemptMove( dt );

		if( ( info.playerPos - GetRect().GetCenter() )
			.GetLengthSq() < shotgunRange * shotgunRange )
		{
			const auto bulletVel = info.playerPos - pos;
			for( int i = -2; i < 3; ++i )
			{
				const auto vel = bulletVel.GetDeviated(
					bulletSpacing * float( i ) / 5.0f );

				pBulletVec->emplace_back( std::make_unique<
					Bullet>( GetRect().GetCenter(),
					GetRect().GetCenter() + vel,*map,
					Bullet::Team::WingedBug,bulletSpeed,
					Bullet::Size::Medium ) );
			}

			EnemyBase::ResetTargeting( jumpRange,jumpSpeed );
			action = State::Wander;
		}
		break;
	case State::Explode:
		if( !exploding.IsFinished() ) exploding.Update( dt );
		if( exploding.IsFinished() ) exploding.SetFrame( 3 );
		break;
	}

	if( explLastFrame )
	{
		explLastFrame = false;

		info.doors.clear();

		// spawn shop/exit door
		// shop key spawns opposite side of room from player
	}
}

void WingedBugMidboss::Draw( Graphics& gfx ) const
{
	switch( action )
	{
	case State::Stay:
		if( IsFlashing() ) floating.Draw( Vei2( pos ),gfx,FlashCol(),vel.x < 0.0f );
		else floating.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		break;
	case State::Wander:
		if( IsFlashing() ) buildingUp.Draw( Vei2( pos ),gfx,FlashCol(),vel.x < 0.0f );
		else buildingUp.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		break;
	case State::BuildUp:
		if( IsFlashing() ) buildingUp.Draw( Vei2( pos ),gfx,FlashCol(),vel.x < 0.0f );
		else buildingUp.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		break;
	case State::ChargeAttack:
		if( IsFlashing() ) charging.Draw( Vei2( pos ),gfx,FlashCol(),vel.x < 0.0f );
		else charging.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		break;
	case State::Explode:
		exploding.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		break;
	}
}

void WingedBugMidboss::Attack( int damage,const Vec2& loc )
{
	EnemyBase::Attack( damage,loc );

	if( IsExpl() )
	{
		explLastFrame = true;
		action = State::Explode;
		coll.MoveTo( Vec2{ -9999.0f,-9999.0f } );
	}
}

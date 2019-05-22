#include "WingedBugMidboss.h"

WingedBugMidboss::WingedBugMidboss( const Vec2& pos,const TileMap& map,
	std::vector<std::unique_ptr<Bullet>>& bullets )
	:
	EnemyBase( pos,size,health,map ),
	floating( 0,0,size.x,size.y,4,*surfSheet,0.2f ),
	buildingUp( 0,size.y,size.x,size.y,4,*surfSheet,0.2f ),
	charging( 0,size.y * 2,size.x,size.y,4,*surfSheet,0.2f ),
	exploding( 0,size.y * 3,size.x,size.y,4,*surfSheet,0.2f )
{
	EnemyBase::ResetTargeting( jumpRange,jumpSpeed );
}

void WingedBugMidboss::Update( const EnemyUpdateInfo& info,float dt )
{
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
		buildingUp.Update( dt );
		buildingUp.SetFrame( 3 );
		if( curJumps < nJumps )
		{
			EnemyBase::Wander( jumpRange,jumpSpeed,dt );

			if( reachedTarget )
			{
				++curJumps;
				action = State::Stay;
			}
		}
		else
		{
			EnemyBase::ResetTargeting( jumpRange,jumpSpeed );
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
		break;
	case State::Explode:
		break;
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
}

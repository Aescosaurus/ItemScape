#include "RynoChaser.h"

RynoChaser::RynoChaser( const Vec2& pos,const TileMap& map,
	std::vector<std::unique_ptr<Bullet>>& bullets )
	:
	EnemyBase( pos,size,health,map ),
	pBulletVec( &bullets ),
	wandering( 0,0,size.x,size.y,4,*sprSheet,0.2f ),
	chargingUp( 0,size.y,size.x,size.y,4,*sprSheet,0.2f ),
	chargingPlayer( 0,size.y * 2,size.x,size.y,4,*sprSheet,0.2f ),
	exploding( 0,size.y * 3,size.x,size.y,4,*sprSheet,0.2f )
{
	ResetTargeting( moveTolerance,speed );
}

void RynoChaser::Update( const EnemyUpdateInfo& info,float dt )
{
	EnemyBase::Update( info,dt );

	switch( action )
	{
	case State::Wander:
		EnemyBase::Wander( moveTolerance,speed,dt );
		
		wanderPeriod.Update( dt );
		wandering.Update( dt );
		if( wanderPeriod.IsDone() && wandering.IsFinished() )
		{
			wanderPeriod.ResetRng();
			wandering.Reset();

			action = State::ChargeUp;
		}
		break;
	case State::ChargeUp:
		chargingUp.Update( dt );
		if( chargingUp.IsFinished() )
		{
			chargingUp.Reset();
			target = info.playerPos;
			vel = ( target - pos ).GetNormalized() * fastSpeed;
			action = State::ChargePlayer;
		}
		break;
	case State::ChargePlayer:
	{
		chargingPlayer.Update( dt );

		const auto testMove = vel * dt;
		const auto validMove = coll.GetValidMove( pos,testMove );
		pos += validMove;
		coll.MoveTo( pos );

		timeBetweenShots.Update( dt );
		if( timeBetweenShots.IsDone() )
		{
			timeBetweenShots.Reset();
			const auto center = GetCenter();
			const auto pi = 3.141592f;
			pBulletVec->emplace_back( std::make_unique<Bullet>(
				center,center + vel.GetDeviated( pi / 2.0f ),
				*map,Bullet::Team::RynoChaser,
				bulletSpeed,Bullet::Size::Small ) );
			pBulletVec->emplace_back( std::make_unique<Bullet>(
				center,center + vel.GetDeviated( -pi / 2.0f ),
				*map,Bullet::Team::RynoChaser,
				bulletSpeed,Bullet::Size::Small ) );
		}

		attackDuration.Update( dt );
		if( attackDuration.IsDone() || validMove.z )
		{
			attackDuration.Reset();
			timeBetweenShots.Reset();
			chargingPlayer.Reset();

			ResetTargeting( moveTolerance,speed );
			action = State::Wander;
		}
	}
		break;
	case State::Explode:
		if( !exploding.IsFinished() ) exploding.Update( dt );
		if( exploding.IsFinished() ) exploding.SetFrame( 3 );
		break;
	}
}

void RynoChaser::Draw( Graphics& gfx ) const
{
	switch( action )
	{
	case State::Wander:
		if( !IsFlashing() ) wandering.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		else wandering.Draw( Vei2( pos ),gfx,FlashCol(),vel.x < 0.0f );
		break;
	case State::ChargeUp:
		if( !IsFlashing() ) chargingUp.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		else chargingUp.Draw( Vei2( pos ),gfx,FlashCol(),vel.x < 0.0f );
		break;
	case State::ChargePlayer:
		if( !IsFlashing() ) chargingPlayer.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		else chargingPlayer.Draw( Vei2( pos ),gfx,FlashCol(),vel.x < 0.0f );
		break;
	case State::Explode:
		exploding.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		break;
	}
}

void RynoChaser::Attack( int damage,const Vec2& loc )
{
	EnemyBase::Attack( damage,loc );

	if( EnemyBase::IsExpl() )
	{
		coll.MoveTo( Vec2{ -9999.0f,-9999.0f } );
		action = State::Explode;
	}
}

Vec2 RynoChaser::GetCenter() const
{
	return( pos + Vec2( size ) / 2.0f );
}

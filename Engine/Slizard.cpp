#include "Slizard.h"

Slizard::Slizard( const Vec2& pos,const TileMap& map,
	std::vector<std::unique_ptr<Bullet>>& bullets )
	:
	EnemyBase( pos,size,health,map ),
	pBulletVec( &bullets ),
	walking( 0,0,size.x,size.y,4,*surfSheet,0.2f ),
	charging( 0,size.y,size.x,size.y,4,*surfSheet,0.2f ),
	attacking( 0,size.y * 2,size.x,size.y,4,*surfSheet,0.2f ),
	exploding( 0,size.y * 3,size.x,size.y,4,*surfSheet,0.2f )
{
	ResetTargeting( moveTolerance,speed );
}

void Slizard::Update( const EnemyUpdateInfo& info,float dt )
{
	EnemyBase::Update( info,dt );

	switch( action )
	{
	case State::Wander:
		Wander( moveTolerance,speed,dt );

		walking.Update( dt );
		if( walking.IsFinished() )
		{
			// This is nested so we only check every time
			//  the anim is finished.  I know the && does
			//  that already but I don't trust it...
			if( !CheckLineOfSight( pos,info.playerPos ) )
			{
				walking.Reset(); // Just to make sure.
				shotTarget = info.playerPos;
				action = State::Charge;
			}
		}
		break;
	case State::Charge:
		charging.Update( dt );
		if( charging.IsFinished() )
		{
			charging.Reset();
			action = State::Phase1Attack;
		}
		break;
	case State::Phase1Attack:
		attacking.Update( dt );

		// Oh boy here we go...
		phase1Refire.Update( dt );
		if( phase1Refire.IsDone() )
		{
			phase1Refire.Reset();
			++curPhase1Shots;

			const float startAngle = ( info.playerPos - pos ).GetAngle();
			const float moveAmount = 3.141592f / 3.5f / float( nPhase1Shots );
			const float curAngle = moveAmount * float( curPhase1Shots );

			ShootBullet( startAngle - curAngle );
			ShootBullet( startAngle + curAngle );
		}

		if( curPhase1Shots >= nPhase1Shots )
		{
			curPhase1Shots = 0;
			phase1Refire.Reset();
			action = State::Phase2Attack;
		}
		break;
	case State::Phase2Attack:
		attacking.Update( dt );

		phase2Refire.Update( dt );
		if( phase2Refire.IsDone() )
		{
			phase2Refire.Reset();
			++curPhase2Shots;

			const float startAngle = ( info.playerPos - pos ).GetAngle();
			const float moveAmount = 3.141592f / 3.5f / float( nPhase2Shots );
			const float curAngle = moveAmount * float( curPhase2Shots );

			ShootBullet( startAngle - curAngle );
			ShootBullet( startAngle + curAngle );
		}

		if( curPhase2Shots >= nPhase2Shots )
		{
			curPhase2Shots = 0;
			phase2Refire.Reset();
			attacking.Reset();
			ResetTargeting( moveTolerance,speed );
			action = State::Wander;
		}
		break;
	case State::Explode:
		if( !exploding.IsFinished() ) exploding.Update( dt );
		if( exploding.IsFinished() ) exploding.SetFrame( 3 );
		break;
	}
}

void Slizard::Draw( Graphics& gfx ) const
{
	switch( action )
	{
	case State::Wander:
		if( !IsFlashing() ) walking.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		else walking.Draw( Vei2( pos ),gfx,FlashCol(),vel.x < 0.0f );
		break;
	case State::Charge:
		if( !IsFlashing() ) charging.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		else charging.Draw( Vei2( pos ),gfx,FlashCol(),vel.x < 0.0f );
		break;
	case State::Phase1Attack:
	case State::Phase2Attack:
		if( !IsFlashing() ) attacking.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		else attacking.Draw( Vei2( pos ),gfx,FlashCol(),vel.x < 0.0f );
		break;
	case State::Explode:
		exploding.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		break;
	}
}

void Slizard::Attack( int damage,const Vec2& loc )
{
	EnemyBase::Attack( damage,loc );

	if( IsExpl() )
	{
		action = State::Explode;
		coll.MoveTo( Vec2{ -9999.0f,-9999.0f } );
	}
}

void Slizard::ShootBullet( float angle )
{
	const Vec2 shotDir = Vec2::FromAngle( angle );
	
	const auto shotPos = GetCenter();
	const auto shotTarget = shotPos + shotDir;
	pBulletVec->emplace_back( std::make_unique<Bullet>(
		shotPos,shotTarget,*map,Bullet::Team::Slizard,
		bulletSpeed,Bullet::Size::Medium ) );
}

bool Slizard::CheckLineOfSight( const Vec2& start,const Vec2& end ) const
{
	auto p0 = Vec2( map->GetTilePos( start ) );
	auto p1 = Vec2( map->GetTilePos( end ) );

	const auto diff = ( end - start ).GetNormalized();
	const auto m = ( p0.x != p1.x )
		? ( p1.y - p0.y ) / ( p1.x - p0.x ) : 0.0f;

	if( p0.x != p1.x && abs( m ) <= 1.0f ) // x bias.
	{
		if( p0.x > p1.x ) std::swap( p0,p1 );

		const float b = p0.y - m * p0.x;

		for( int x = int( p0.x ); x < int( p1.x ); ++x )
		{
			const float y = m * float( x ) + b;
			if( map->GetTile( x,int( y ) ) ==
				TileMap::TileType::Wall )
			{
				return( true );
			}
		}
	}
	else // y bias.
	{
		if( p0.y > p1.y ) std::swap( p0,p1 );

		const float w = ( p1.x - p0.x ) / ( p1.y - p0.y );
		const float p = p0.x - w * p0.y;

		for( int y = int( p0.y ); y < int( p1.y ); ++y )
		{
			const float x = w * float( y ) + p;
			if( map->GetTile( int( x ),y ) ==
				TileMap::TileType::Wall )
			{
				return( true );
			}
		}
	}

	return( false );
}

Vec2 Slizard::GetCenter() const
{
	return( pos + Vec2( size ) / 2.0f );
}

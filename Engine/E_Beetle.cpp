#include "E_Beetle.h"
#include "Random.h"

Beetle::Beetle( const Vec2& pos,const TileMap& map,
	std::vector<std::unique_ptr<Bullet>>& myBullets )
	:
	EnemyBase( pos,size,maxHP,map ),
	walking( 0,0,size.x,size.y,4,*pSprSheet,0.2f ),
	firing( 0,size.y,size.x,size.y,4,*pSprSheet,0.2f ),
	windDown( 0,size.y * 2,size.x,size.y,4,*pSprSheet,0.2f ),
	explAnim( 0,size.y * 3,size.x,size.y,4,*pSprSheet,0.2f ),
	shotTarget( 0.0f,0.0f ),
	pBulletVec( &myBullets ),
	map( &map )
{
	ResetTargeting();
	ResetTimer();
}

void Beetle::Update( const EnemyUpdateInfo& info,float dt )
{
	switch( myAction )
	{
	case State::Moving:
	{
		// Move towards target.
		const auto testMove = vel * dt;
		const auto validMove = coll.GetValidMove( pos,testMove );
		pos += validMove;
		coll.MoveTo( pos );

		if( map->GetTilePos( target ) ==
			map->GetTilePos( pos ) ||
			validMove.z ) // Tells if you collided or not.
		{
			ResetTargeting();
		}

		walking.Update( dt );
		shotTimer.Update( dt );
		if( walking.IsFinished() && shotTimer.IsDone() )
		{
			ResetTimer();
			walking.Reset();
			myAction = State::Firing;
		}
		break;
	}
	case State::Firing:
		firing.Update( dt );
		shotTarget = info.playerPos;
		if( firing.IsFinished() )
		{
			pBulletVec->emplace_back( std::make_unique<Bullet>( pos,info.playerPos,
				*map,Bullet::Team::Beetle,myBulletSpeed,
				Bullet::Size::Small ) );

			firing.Reset();
			myAction = State::WindingDown;
		}
		break;
	case State::WindingDown:
		windDown.Update( dt );
		if( windDown.IsFinished() )
		{
			windDown.Reset();
			myAction = State::Moving;
		}
		break;
	case State::Exploding:
		// Play explode anim and stop at last frame.
		if( !explAnim.IsFinished() ) explAnim.Update( dt );
		if( explAnim.IsFinished() )
		{
			explAnim.SetFrame( 3 );
			if( !deadDead )
			{
				deadDead = true;
				explDrawDir = ( Random::RangeI( 0,10 ) > 5 );
			}
		}
		break;
	}
}

void Beetle::Draw( Graphics& gfx ) const
{
	switch( myAction )
	{
	case State::Moving:
		walking.Draw( pos,gfx,vel.x < 0.0f );
		break;
	case State::Firing:
		firing.Draw( pos,gfx,shotTarget.x < GetCenter().x );
		break;
	case State::WindingDown:
		windDown.Draw( pos,gfx,shotTarget.x < GetCenter().x );
		break;
	case State::Exploding:
		explAnim.Draw( pos,gfx,explDrawDir );
		break;
	}
	// gfx.DrawHitbox( coll.GetRect(),Colors::Green );
}

void Beetle::Attack( int damage,const Vec2& loc )
{
	EnemyBase::Attack( damage,loc );

	// TODO: Make it get pushed away from bullet.

	if( IsDead() )
	{
		myAction = State::Exploding;
		coll.MoveTo( Vec2{ -9999.0f,-9999.0f } );
	}
}

void Beetle::ResetTargeting()
{
	target = FindTarget();
	lastTarget = target;
	vel = ( target - pos ).GetNormalized() * speed;
}

void Beetle::ResetTimer()
{
	shotTimer.Reset( Random::RangeF( timerMinMax.x,timerMinMax.y ) );
}

Vec2 Beetle::FindTarget() const
{
	auto test = Vec2( pos );
	const int nTries = 5;
	int curTries = 0;
	do
	{
		test = Vec2( pos );
		test.x += Random::RangeF( -moveTolerance,moveTolerance );
		test.y += Random::RangeF( -moveTolerance,moveTolerance );

		++curTries;
		if( curTries >= nTries ) // No infinite loops here!
		{
			test = lastTarget;
			break;
		}
	}
	while( !Graphics::GetScreenRect()
		.ContainsPoint( Vei2( Vec2{ test.x,test.y } ) ) );

	return( test );
}

Vec2 Beetle::GetCenter() const
{
	return( pos + Vec2( size ) / 2.0f );
}

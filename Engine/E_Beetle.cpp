#include "E_Beetle.h"
#include "Random.h"

Beetle::Beetle( const Vec2& pos,const TileMap& map,
	std::vector<Bullet>& myBullets )
	:
	EnemyBase( pos,size,maxHP,map ),
	walking( 0,0,size.x,size.y,4,*pSprSheet,0.2f ),
	firing( 0,size.y,size.x,size.y,4,*pSprSheet,0.2f ),
	windDown( 0,size.y * 2,size.x,size.y,4,*pSprSheet,0.2f ),
	coll( map,{ pos,float( size.x ),float( size.y ) } ),
	shotTarget( 0.0f,0.0f ),
	pBulletVec( &myBullets ),
	map( &map )
{
	ResetTargeting();
}

void Beetle::Update( const Vec2& playerPos,float dt )
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
			shotTimer.Reset();
			walking.Reset();
			myAction = State::Firing;
		}
		break;
	}
	case State::Firing:
		firing.Update( dt );
		shotTarget = playerPos;
		if( firing.IsFinished() )
		{
			pBulletVec->emplace_back( Bullet{ pos,playerPos,
				*map,Bullet::Team::Beetle,myBulletSpeed } );

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
	}
	gfx.DrawHitbox( coll.GetRect() );
}

void Beetle::ResetTargeting()
{
	target = FindTarget();
	vel = ( target - pos ).GetNormalized() * speed;
}

Vec2 Beetle::FindTarget() const
{
	auto test = Vec2( pos );
	do
	{
		test.x += Random::RangeF( -moveTolerance,moveTolerance );
		test.y += Random::RangeF( -moveTolerance,moveTolerance );
	}
	while( !Graphics::GetScreenRect()
		.ContainsPoint( Vei2( Vec2{ test.x,test.y } ) ) );

	return( test );
}

Vec2 Beetle::GetCenter() const
{
	return( pos + Vec2( size ) / 2.0f );
}

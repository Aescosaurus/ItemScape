#include "EnemyBase.h"
#include "Random.h"

void EnemyBase::Attack( int damage,const Vec2& loc )
{
	hp -= damage;
}

bool EnemyBase::IsExpl() const
{
	return( hp < 1 );
}

const Rect& EnemyBase::GetRect() const
{
	return( coll.GetRect() );
}

EnemyBase::EnemyBase( const Vec2& pos,const Vec2& size,
	int hp,const TileMap& map )
	:
	pos( pos ),
	coll( map,{ pos,size.x,size.y } ),
	hp( hp ),
	map( &map )
{}

void EnemyBase::Wander( Vec2& target,Vec2& lastTarget,
	Vec2& vel,float moveTolerance,float speed,float dt )
{
	const auto testMove = vel * dt;
	const auto validMove = coll.GetValidMove( pos,testMove );
	pos += validMove;
	coll.MoveTo( pos );

	if( map->GetTilePos( target ) ==
		map->GetTilePos( pos ) ||
		validMove.z )
	{
		ResetTargeting( target,lastTarget,vel,
			moveTolerance,speed );
	}
}

void EnemyBase::ResetTargeting( Vec2& target,Vec2& lastTarget,
	Vec2& vel,float moveTolerance,float speed )
{
	Vec2 newTarget;
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
		} while( !Graphics::GetScreenRect()
			.ContainsPoint( Vei2( Vec2{ test.x,test.y } ) ) );

		newTarget = test;
	}

	target = newTarget;
	lastTarget = target;
	vel = ( target - pos ).GetNormalized() * speed;
}

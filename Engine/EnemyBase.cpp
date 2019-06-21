#include "EnemyBase.h"
#include "Random.h"

void EnemyBase::Update( const EnemyUpdateInfo& info,float dt )
{
	damageCooldown.Update( dt );
}

void EnemyBase::Attack( int damage,const Vec2& loc )
{
	hp -= damage;

	damageCooldown.Reset();

	if( !IsBoss() )
	{
		const auto testMove = ( pos - loc ).GetNormalized() *
			damagePushDist * float( damage );
		const auto validMove = coll.GetValidMove( pos,testMove );
		pos += validMove;
		coll.MoveTo( pos );
	}

	if( IsExpl() ) coll.MoveTo( Vec2{ -9999.0f,-9999.0f } );
}

bool EnemyBase::IsExpl() const
{
	return( hp < 1 );
}

const Vec2& EnemyBase::GetPos() const
{
	return( pos );
}

const Rect& EnemyBase::GetRect() const
{
	return( coll.GetRect() );
}

float EnemyBase::GetHPPercent() const
{
	return( float( hp ) / float( maxHP ) );
}

int EnemyBase::GetCurHP() const
{
	return( hp );
}

int EnemyBase::GetMaxHP() const
{
	return( maxHP );
}

bool EnemyBase::IsBoss() const
{
	return( false );
}

EnemyBase::EnemyBase( const Vec2& pos,const Vec2& size,
	int hp,const TileMap& map )
	:
	pos( pos ),
	coll( map,{ pos,size.x,size.y } ),
	hp( hp ),
	maxHP( hp ),
	map( &map )
{
	damageCooldown.Update( damageCooldown.GetDuration() );
}

void EnemyBase::Wander( float moveTolerance,float speed,float dt )
{
	const auto testMove = vel * dt;
	const auto validMove = coll.GetValidMove( pos,testMove );
	pos += validMove;
	coll.MoveTo( pos );

	if( map->GetTilePos( target ) ==
		map->GetTilePos( pos ) ||
		validMove.z )
	{
		ResetTargeting( moveTolerance,speed );
		reachedTarget = true;
	}
	else
	{
		reachedTarget = false;
	}
}

void EnemyBase::ResetTargeting( float moveTolerance,float speed )
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

void EnemyBase::AttemptMove( float dt )
{
	const auto testMove = vel * dt;
	const auto validMove = coll.GetValidMove( pos,testMove );
	pos += validMove;
	coll.MoveTo( pos );
}

SpriteEffect::Substitution EnemyBase::FlashCol() const
{
	return( SpriteEffect::Substitution{ Colors::Magenta,Colors::White } );
}

bool EnemyBase::IsFlashing() const
{
	return( !damageCooldown.IsDone() );
}

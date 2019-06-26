#include "EnemyBase.h"
#include "Random.h"

void EnemyBase::UpdateBase( const EnemyUpdateInfo& info,float dt )
{
	const auto nBullets = int( info.enemyBullets.size() );
	const Vec2 stuckPos = pos;

	if( !HasEffect( Effect::Frozen ) )
	{
		Update( info,dt ); // Calls child update.
	}

	while( HasEffect( Effect::Stunned ) &&
		int( info.enemyBullets.size() ) > nBullets )
	{
		info.enemyBullets.pop_back();
	}
	if( HasEffect( Effect::Stuck ) ) pos = stuckPos;
	if( HasEffect( Effect::Confused ) )
	{
		for( int i = nBullets;
			i < int( info.enemyBullets.size() );
			++i )
		{
			const auto curVel = info.enemyBullets[i]
				->GetVel().GetLength();
			info.enemyBullets[i]->SetVel( Vec2{
				Random::RangeF( -1.0f,1.0f ),
				Random::RangeF( -1.0f,1.0f ) }
				.GetNormalized() * curVel );
		}
	}

	for( auto it = effects.begin(); it != effects.end(); ++it )
	{
		it->second.Update( dt );
		if( it->second.IsDone() )
		{
			it = effects.erase( it );
			if( it == effects.end() ) break;
		}
	}
}

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

void EnemyBase::ApplyEffect( Effect eff,float duration )
{
	effects.emplace_back( std::make_pair( eff,Timer{ duration } ) );
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

bool EnemyBase::HasEffect( Effect eff ) const
{
	for( const auto& e : effects )
	{
		if( e.first == eff )
		{
			return( true );
		}
	}
	return( false );
}

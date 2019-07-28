#include "HolyCrab.h"
#include "Utils.h"

HolyCrab::HolyCrab( const Vec2& pos,const TileMap& map,
	std::vector<std::unique_ptr<Bullet>>& bulletVec )
	:
	EnemyBase( pos,size,myHP,map ),
	pBulletVec( &bulletVec ),
	wandering( 0,0,size.x,size.y,4,*sprSheet,0.2f ),
	windingUp( 0,size.y,size.x,size.y,4,*sprSheet,0.2f ),
	spinAttacking( 0,size.y * 2,size.x,size.y,4,*sprSheet,0.2f ),
	exploding( 0,size.y * 3,size.x,size.y,4,*sprSheet,0.2f )
{
	EnemyBase::ResetTargeting( moveRange,wanderSpeed );
	wanderTimer.Update( Random::RangeF( 0.0f,
		wanderTimer.GetDuration() / 2.0f ) );
}

void HolyCrab::Update( const EnemyUpdateInfo& info,float dt )
{
	switch( action )
	{
	case State::Wander:
		Wander( moveRange,wanderSpeed,dt );
		wandering.Update( dt );
		wanderTimer.Update( dt );
		if( wanderTimer.IsDone() && wandering.IsFinished() )
		{
			wanderTimer.Reset();
			wandering.Reset();
			action = State::WindUp;
		}
		break;
	case State::WindUp:
		windingUp.Update( dt );
		if( windingUp.IsFinished() )
		{
			windingUp.Reset();
			action = State::SpinAttack;
		}
		break;
	case State::SpinAttack:
		spinAttacking.Update( dt );
		
		const auto bulletSpawnPos = pos + Vec2( size ) / 2.0f;
		shotTimer.Update( dt );
		if( shotTimer.IsDone() )
		{
			shotTimer.Reset();

			const auto vel = Vec2{ 0.0f,-1.0f }
				.Deviate( ( chili::pi * 2.0f ) *
				( float( curShot ) / float( shotsPerVolley ) ) );

			pBulletVec->emplace_back( std::make_unique<Bullet>(
				bulletSpawnPos,bulletSpawnPos + vel,*map,
				Bullet::Team::HolyCrab,bulletSpeed,
				Bullet::Size::Small ) );
			pBulletVec->emplace_back( std::make_unique<Bullet>(
				bulletSpawnPos,bulletSpawnPos - vel,*map,
				Bullet::Team::HolyCrab,bulletSpeed,
				Bullet::Size::Small ) );

			++curShot;
			if( curShot > shotsPerVolley )
			{
				curShot = 0;
				action = State::Wander;
			}
		}
		break;
	case State::Exploding:
		break;
	}
}

void HolyCrab::Draw( Graphics& gfx ) const
{
	switch( action )
	{
	case State::Wander:
		if( !IsFlashing() ) wandering.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		else wandering.Draw( Vei2( pos ),gfx,FlashCol(),vel.x < 0.0f );
		break;
	case State::WindUp:
		if( !IsFlashing() ) windingUp.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		else windingUp.Draw( Vei2( pos ),gfx,FlashCol(),vel.x < 0.0f );
		break;
	case State::SpinAttack:
		if( !IsFlashing() ) spinAttacking.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		else spinAttacking.Draw( Vei2( pos ),gfx,FlashCol(),vel.x < 0.0f );
		break;
	case State::Exploding:
		exploding.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		break;
	}
}

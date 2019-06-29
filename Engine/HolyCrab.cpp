#include "HolyCrab.h"

HolyCrab::HolyCrab( const Vec2& pos,const TileMap& map,
	std::vector<std::unique_ptr<Bullet>>& bulletVec )
	:
	EnemyBase( pos,size,myHP,map ),
	pBulletVec( &bulletVec )
{
	EnemyBase::ResetTargeting( moveRange,wanderSpeed );
}

void HolyCrab::Update( const EnemyUpdateInfo& info,float dt )
{
	switch( action )
	{
	case State::Wander:
		break;
	case State::WindUp:
		break;
	case State::SpinAttack:
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
		break;
	case State::WindUp:
		break;
	case State::SpinAttack:
		break;
	case State::Exploding:
		break;
	}
}

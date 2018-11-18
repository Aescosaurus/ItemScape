#include "RynoChaser.h"

RynoChaser::RynoChaser( const Vec2& pos,const TileMap& map,
	std::vector<std::unique_ptr<Bullet>>& bullets )
	:
	EnemyBase( pos,size,health,map ),
	pBulletVec( &bullets )
{}

void RynoChaser::Update( const EnemyUpdateInfo& info,float dt )
{
	switch( action )
	{
	case State::Wander:
		break;
	case State::ChargeUp:
		break;
	case State::ChargePlayer:
		break;
	case State::Explode:
		break;
	}
}

void RynoChaser::Draw( Graphics& gfx ) const
{
	switch( action )
	{
	case State::Wander:
		break;
	case State::ChargeUp:
		break;
	case State::ChargePlayer:
		break;
	case State::Explode:
		break;
	}
}

void RynoChaser::Attack( int damage,const Vec2& loc )
{
	EnemyBase::Attack( damage,loc );


}
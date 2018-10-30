#include "E__EnemyBase.h"

void EnemyBase::Attack( int damage,const Vec2& loc )
{
	hp -= damage;
}

bool EnemyBase::IsDead() const
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

void EnemyBase::TryMovement( const Vec2& testMove )
{
	pos += coll.GetValidMove( pos,testMove );
}

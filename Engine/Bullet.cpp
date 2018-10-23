#include "Bullet.h"

Bullet::Bullet( const Vec2& pos,const Vec2& target,
	const TileMap& map,Team myTeam,float mySpeed )
	:
	pos( pos ),
	vel( ( target - pos ).GetNormalized() * mySpeed ),
	myTeam( myTeam ),
	myAnim( 0,int( myTeam ) * size.y,size.x,size.y,4,*pSprSheet,0.2f ),
	map( &map )
{}

void Bullet::Update( float dt )
{
	pos += vel * dt;
	
	myAnim.Update( dt );

	if( map->GetTileAt( pos ) == TileMap::TileType::Wall )
	{
		dead = true;
	}
}

void Bullet::Draw( Graphics& gfx ) const
{
	myAnim.Draw( pos,gfx );
}

bool Bullet::IsDead() const
{
	return( dead );
}

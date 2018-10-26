#include "Bullet.h"

void Bullet::Update( float dt )
{
	pos += vel * dt;
}

bool Bullet::IsDead() const
{
	return( dead );
}

Bullet::Bullet( const Vec2& pos,const Vec2& target,
	const TileMap& map,Team myTeam,float speed )
	:
	pos( pos ),
	vel( ( target - pos ).GetNormalized() * speed ),
	myTeam( myTeam ),
	map( &map )
{}

BulletS::BulletS( const Vec2& pos,const Vec2& target,
	const TileMap& map,Team myTeam,float speed )
	:
	Bullet( pos,target,map,myTeam,speed ),
	myAnim( 0,int( myTeam ) * size.y,size.x,size.y,4,*pSprSheet,0.2f )
{}

void BulletS::Update( float dt )
{
	Bullet::Update( dt );

	myAnim.Update( dt );

	if( map->GetTileAt( pos ) == TileMap::TileType::Wall )
	{
		dead = true;
	}
}

void BulletS::Draw( Graphics& gfx ) const
{
	myAnim.Draw( Vei2( pos ),gfx,false );
}

BulletM::BulletM( const Vec2& pos,const Vec2& target,
	const TileMap& map,Team myTeam,float speed )
	:
	Bullet( pos,target,map,myTeam,speed ),
	myAnim( 0,int( myTeam ) * size.y,size.x,size.y,4,*pSprSheet,0.2f )
{}

void BulletM::Update( float dt )
{
	Bullet::Update( dt );

	myAnim.Update( dt );

	if( map->GetTileAt( pos ) == TileMap::TileType::Wall )
	{
		dead = true;
	}
}

void BulletM::Draw( Graphics& gfx ) const
{
	myAnim.Draw( Vei2( pos ),gfx,false );
}

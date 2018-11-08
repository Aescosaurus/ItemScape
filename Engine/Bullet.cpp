#include "Bullet.h"

Bullet::Bullet( const Vec2& pos,const Vec2& target,
	const TileMap& map,Team myTeam,float speed,Size mySize )
	:
	pos( pos ),
	vel( ( target - pos ).GetNormalized() * speed ),
	myTeam( myTeam ),
	mySize( mySize ),
	map( &map ),
	// This is useless, make it for real in the body.
	myAnim( 0,0,0,0,0,*pSprSheet,0.2f ),
	coll( map,{ { 0,0 },0,0 } )
{
	Vei2 animStart = { 0,0 };
	Vei2 frameSize = { 0,0 };
	static constexpr int nFrames = 4;
	static constexpr float animSpeed = 0.2f;

	switch( mySize )
	{
	case Size::Small:
		animStart.x = 0;
		animStart.y = size.y * int( myTeam );
		frameSize = size;
		break;
	case Size::Medium:
		animStart.x = size.x * 4;
		animStart.y = size.y * ( int( myTeam ) - int( Team::LastOfSmall ) - 1 );
		frameSize = size * int( Size::Medium );
		break;
	}

	coll = Collider{ map,{ pos,float( frameSize.x ),float( frameSize.y ) } };

	myAnim = Anim{ animStart.x,animStart.y,
		frameSize.x,frameSize.y,nFrames,*pSprSheet,
		animSpeed };
}

void Bullet::Update( float dt )
{
	const auto testMove = vel * dt;
	const auto validMove = coll.GetValidMove( pos,testMove );

	pos += Vec2( validMove );
	coll.MoveTo( pos - Vec2( size ) / 2.0f );

	if( validMove.z ) dead = true;

	myAnim.Update( dt );
}

void Bullet::Draw( Graphics& gfx ) const
{
	myAnim.Draw( Vei2( pos - Vec2( size ) / 2.0f ),gfx,false );
	
	gfx.DrawHitbox( coll.GetRect() );
}

void Bullet::Attack( int damage )
{
	dead = true;
}

bool Bullet::IsDead() const
{
	return( dead );
}

const Rect& Bullet::GetRect() const
{
	return( coll.GetRect() );
}

#include "Bullet.h"
#include "SpriteEffect.h"

Bullet::Bullet( const Vec2& pos,const Vec2& target,
	const TileMap& map,Team myTeam,float speed,
	Size mySize,int damage )
	:
	pos( pos ),
	vel( ( target - pos ).GetNormalized() * speed ),
	myTeam( myTeam ),
	mySize( mySize ),
	map( &map ),
	// This is useless, make it for real in the body.
	myAnim( 0,0,0,0,0,*pSprSheet,0.2f ),
	coll( map,{ { 0,0 },0,0 } ),
	damage( damage )
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
		animStart.y = 2 * size.y * ( int( myTeam ) - int( Team::LastOfSmall ) - 1 );
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
	if( subColor != Colors::Magenta )
	{
		myAnim.Draw( Vei2( pos - Vec2( size ) / 2.0f ),
			gfx,SpriteEffect::SubstituteFade{
				Colors::Magenta,subColor,0.5f } );
	}
	else
	{
		myAnim.Draw( Vei2( pos - Vec2( size ) / 2.0f ),gfx,false );
	}
	
	// gfx.DrawHitbox( coll.GetRect() );
}

void Bullet::Attack( int damage )
{
	dead = true;
}

void Bullet::SetSubColor( Color c )
{
	subColor = c;
}

Bullet* Bullet::Clone()
{
	return( new Bullet{ *this } );
}

bool Bullet::IsExpl() const
{
	return( dead );
}

Vec2& Bullet::GetPos()
{
	return( pos );
}

const Rect& Bullet::GetRect() const
{
	return( coll.GetRect() );
}

int& Bullet::GetDamage()
{
	return( damage );
}

Vec2& Bullet::GetVel()
{
	return( vel );
}

WavyBullet::WavyBullet( const Bullet& src )
	:
	Bullet( src ),
	normVel( vel.GetNormalized() ),
	speed( vel.GetLength() )
{}

void WavyBullet::Update( float dt )
{
	auto testMove = normVel;

	const Vec2 wavyMoveAdd = Vec2( testMove.y,-testMove.x ) *
		sin( distTravelled * freq ) * amplitude;
	testMove.x += /*cos*/( wavyMoveAdd.x );
	testMove.y += /*sin*/( wavyMoveAdd.y );

	testMove *= speed;
	testMove *= dt;

	const auto validMove = coll.GetValidMove( pos,testMove );

	pos += Vec2( validMove );
	coll.MoveTo( pos - Vec2( size ) / 2.0f );

	if( validMove.z ) dead = true;

	myAnim.Update( dt );

	distTravelled += dt;
}

Bullet* WavyBullet::Clone()
{
	return( new WavyBullet{ *this } );
}

TrackingBullet::TrackingBullet( const Bullet& src )
	:
	Bullet( src )
{}

void TrackingBullet::Update( float dt )
{
	const auto diff = ( ( ( *target ) + offset ) - pos )
		.GetNormalized();

	const auto testMove = diff * speed * dt;
	const auto validMove = coll.GetValidMove( pos,testMove );

	pos += Vec2( validMove );
	coll.MoveTo( pos - Vec2( size ) / 2.0f );

	if( validMove.z ||
		coll.GetRect().ContainsPoint( *target ) )
	{
		dead = true;
	}

	myAnim.Update( dt );
}

void TrackingBullet::SetTarget( const Vec2& target )
{
	this->target = &target;
}

void TrackingBullet::SetSpeed( float speed )
{
	this->speed = speed;
}

void TrackingBullet::SetOffset( const Vec2& offset )
{
	this->offset = offset;
}

Bullet* TrackingBullet::Clone()
{
	return( new TrackingBullet{ *this } );
}

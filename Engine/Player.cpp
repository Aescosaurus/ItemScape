#include "Player.h"

Player::Player( const Vec2& pos,const TileMap& map,
	std::vector<std::unique_ptr<Bullet>>& bullets )
	:
	pos( pos ),
	coll( map,{ pos,float( size.x ) / 1.5f,float( size.y ) / 1.5f } ),
	myBullets( bullets ),
	map( map )
{
}

void Player::Update( const Keyboard& kbd,const Mouse& ms,
	float dt )
{
	// Change moveDir based on key hits.
	moveDir = { 0.0f,0.0f };
	if( kbd.KeyIsPressed( 'W' ) ) --moveDir.y;
	if( kbd.KeyIsPressed( 'S' ) ) ++moveDir.y;
	if( kbd.KeyIsPressed( 'A' ) ) --moveDir.x;
	if( kbd.KeyIsPressed( 'D' ) ) ++moveDir.x;

	// Move but don't let you walk into walls.
	moveDir = moveDir.GetNormalized() * speed * dt;
	pos += coll.GetValidMove( pos,moveDir );
	coll.MoveTo( pos - coll.GetSize() / 2.0f );

	// Handle super cool jumping mechanic.
	if( kbd.KeyIsPressed( VK_SPACE ) && !jumping &&
		jumpReset.IsDone() )
	{
		jumpReset.Reset();

		Jump();
	}

	if( jumping )
	{
		jumpTimer.Update( dt );
		if( jumpTimer.IsDone() )
		{
			jumpTimer.Reset();
			Land();
		}
	}
	else jumpReset.Update( dt );

	// Deal with shooting.
	shotTimer.Update( dt );
	if( ms.LeftIsPressed() && shotTimer.IsDone() )
	{
		shotTimer.Reset();
		myBullets.emplace_back( std::make_unique<Bullet>(
			pos,Vec2( ms.GetPos() ),map,
			Bullet::Team::Player,bulletSpeed,
			Bullet::Size::Small ) );
	}
}

void Player::Draw( Graphics& gfx ) const
{
	const auto drawPos = Vei2( GetCenter() );
	gfx.DrawRect( drawPos.x,drawPos.y,
		size.x,size.y,Colors::Red );

	if( coll.GetRect().IsContainedBy( Graphics::GetScreenRect() ) )
	{
		gfx.DrawHitbox( coll.GetRect() );
	}

	if( jumping )
	{
		// TODO: Play jumping animation.

		gfx.DrawRect( drawPos.x,drawPos.y,
			size.x,size.y,Colors::Blue );
	}
}

void Player::MoveTo( const Vec2& updatedPos )
{
	pos = updatedPos;
}

const Vec2& Player::GetPos() const
{
	return( pos );
}

const Vec2& Player::GetVel() const
{
	return( moveDir );
}

Vec2 Player::GetCenter() const
{
	return( pos - Vec2( size ) / 2.0f );
}

Rect Player::GetRect() const
{
	if( jumping )
	{
		return( RectI{ coll.GetRect() }.MoveTo( { -9999,-9999 } ) );
	}

	return( coll.GetRect() );
}

void Player::Jump()
{
	jumping = true;

	// coll.MoveTo( { -9999.0f,-9999.0f } );
}

void Player::Land()
{
	jumping = false;

	// coll.MoveTo( pos - coll.GetSize() / 2.0f );
}

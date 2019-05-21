#include "Player.h"
#include "SpriteEffect.h"

Player::Player( const Vec2& pos,const TileMap& map,
	std::vector<std::unique_ptr<Bullet>>& bullets )
	:
	pos( pos ),
	coll( map,{ pos,float( size.x ) / 1.5f,float( size.y ) / 1.5f } ),
	myBullets( bullets ),
	map( map ),
	walk( 0,0,size.x,size.y,4,surfSheet,0.2f )
{}

void Player::Update( const Keyboard& kbd,const Mouse& ms,
	float dt )
{
	// Change moveDir based on key hits.
	moveDir = { 0.0f,0.0f };
	if( kbd.KeyIsPressed( 'W' ) ) --moveDir.y;
	if( kbd.KeyIsPressed( 'S' ) ) ++moveDir.y;
	if( kbd.KeyIsPressed( 'A' ) ) --moveDir.x;
	if( kbd.KeyIsPressed( 'D' ) ) ++moveDir.x;

	if( moveDir == Vec2{ 0.0f,0.0f } ) walk.Reset();
	else walk.Update( dt );

	// Move but don't let you walk into walls.
	moveDir = moveDir.GetNormalized() * speed *
		moveSpeedFactor * dt;
	pos += coll.GetValidMove( pos,moveDir );
	coll.MoveTo( pos - coll.GetSize() / 2.0f );

	// Handle super cool jumping mechanic.
	// if( kbd.KeyIsPressed( VK_SPACE ) && !jumping &&
	// 	jumpReset.IsDone() )
	// {
	// 	jumpReset.Reset();
	// 
	// 	Jump();
	// }
	// 
	// if( jumping )
	// {
	// 	jumpTimer.Update( dt );
	// 	if( jumpTimer.IsDone() )
	// 	{
	// 		jumpTimer.Reset();
	// 		Land();
	// 	}
	// }
	// else jumpReset.Update( dt );

	// Deal with shooting.
	// shotTimer.Update( dt );
	// if( ms.LeftIsPressed() && shotTimer.IsDone() )
	// {
	// 	shotTimer.Reset();
	// 	myBullets.emplace_back( std::make_unique<Bullet>(
	// 		pos,Vec2( ms.GetPos() ),map,
	// 		Bullet::Team::Player1,bulletSpeed,
	// 		Bullet::Size::Small ) );
	// 
	// 	justShot = true;
	// }
	// else justShot = false;
}

void Player::Draw( Graphics& gfx ) const
{
	const auto drawPos = Vei2( GetCenter() );
	// gfx.DrawRect( drawPos.x,drawPos.y,
	// 	size.x,size.y,Colors::Red );

	if( subColor == Colors::Magenta )
	{
		walk.Draw( drawPos,gfx,moveDir.x < 0.0f );
	}
	else
	{
		walk.Draw( drawPos,gfx,SpriteEffect::SubstituteFade{
			Colors::Magenta,subColor,0.5f },moveDir.x < 0.0f );
	}

	// if( coll.GetRect().IsContainedBy( Graphics::GetScreenRect() ) )
	// {
	// 	gfx.DrawHitbox( coll.GetRect() );
	// }

	// if( jumping )
	// {
	// 	// TODO: Play jumping animation.
	// 
	// 	gfx.DrawRect( drawPos.x,drawPos.y,
	// 		size.x,size.y,Colors::Blue );
	// }
}

void Player::MoveTo( const Vec2& updatedPos )
{
	pos = updatedPos;
}

void Player::SetJustShot( bool val )
{
	justShot = val;
}

void Player::SetInvulStatus( bool isInvul )
{
	invul = isInvul;
}

void Player::SetSubColor( Color c )
{
	subColor = c;
}

void Player::MultiplyMoveSpeedFactor( float amount )
{
	moveSpeedFactor *= amount;
}

Vec2& Player::GetPos()
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
	// if( jumping )
	// {
	// 	return( RectI{ coll.GetRect() }.MoveTo( { -9999,-9999 } ) );
	// }

	return( coll.GetRect() );
}

bool Player::JustShot() const
{
	return( justShot );
}

bool Player::IsInvul() const
{
	return( invul );
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

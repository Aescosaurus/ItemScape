#include "Door.h"
#include <cassert>
#include "SpriteEffect.h"

const Surface Door::topSurf = { { { "Images/DoorAngles.bmp" },
	RectI{ 0,16,0,8 } },Vei2{ 4,4 } };
const Surface Door::botSurf = { { { "Images/DoorAngles.bmp" },
	RectI{ 0,16,8,16 } },Vei2{ 4,4 } };
const Surface Door::sideSurf = { { { "Images/DoorAngles.bmp" },
	RectI{ 16,24,0,16 } },Vei2{ 4,4 } };

Door::Door( Side s,const FloorLevel& floor )
	:
	pos( GetPosFromSide( s ) ),
	hitbox( GetRectFromSide( s ) ),
	mySide( s )
{
	UpdateActivated( floor );
}

void Door::Draw( Graphics& gfx ) const
{
	if( activated )
	{
		const auto chroma = SpriteEffect::Chroma{ Colors::Magenta };
		switch( mySide )
		{
		case Side::Top:
			gfx.DrawSprite( pos.x,pos.y,topSurf,chroma );
			break;
		case Side::Bot:
			gfx.DrawSprite( pos.x,pos.y,botSurf,chroma );
			break;
		case Side::Left:
			gfx.DrawSprite( pos.x,pos.y,sideSurf,chroma,false );
			break;
		case Side::Right:
			gfx.DrawSprite( pos.x,pos.y,sideSurf,chroma,true );
			break;
		}
	}
	gfx.DrawHitbox( hitbox );
}

void Door::UpdateActivated( const FloorLevel& floor )
{
	const auto& curRoom = floor.GetCurRoom();
	switch( mySide )
	{
	case Side::Top:
		activated = floor.GetLevelAt( curRoom + Vei2::Up() ) != -1;
		break;
	case Side::Bot:
		activated = floor.GetLevelAt( curRoom + Vei2::Down() ) != -1;
		break;
	case Side::Left:
		activated = floor.GetLevelAt( curRoom + Vei2::Left() ) != -1;
		break;
	case Side::Right:
		activated = floor.GetLevelAt( curRoom + Vei2::Right() ) != -1;
		break;
	}
}

bool Door::IsActivated() const
{
	return( activated );
}

const RectI& Door::GetRect() const
{
	return( hitbox );
}

Door::Side Door::GetSide() const
{
	return( mySide );
}

Vec2 Door::GetPlayerSpawnPos( const Vec2& guyPos ) const
{
	switch( GetSide() )
	{
	case Door::Side::Top:
		return( Vec2{ guyPos.x,float( Graphics::ScreenHeight - 64 ) } );
		break;
	case Door::Side::Bot:
		return( Vec2{ guyPos.x,float( 0 + 64 ) } );
		break;
	case Door::Side::Left:
		return( Vec2{ float( Graphics::ScreenWidth - 64 ),guyPos.y } );
		break;
	case Door::Side::Right:
		return( Vec2{ float( 0 + 64 ),guyPos.y } );
		break;
	}
}

Vei2 Door::GetPosFromSide( Side s ) const
{
	switch( s )
	{
	case Side::Top:
		return( Vei2{ Graphics::ScreenWidth / 2 - dim.x / 2,
			0 + 32 } );
	case Side::Bot:
		return( Vei2{ Graphics::ScreenWidth / 2 - dim.x / 2,
			Graphics::ScreenHeight - 32 - dim.y } );
	case Side::Left:
		return( Vei2{ 0 + 32,
			Graphics::ScreenHeight / 2 - dim.y / 2 } );
	case Side::Right:
		return( Vei2{ Graphics::ScreenWidth - 32 - dim.y,
			Graphics::ScreenHeight / 2 - dim.y / 2 } );
	default:
		assert( false );
		return( Vei2{ -1,-1 } );
		break;
	}
}

RectI Door::GetRectFromSide( Side s ) const
{
	const Vei2 pos = GetPosFromSide( s );
	switch( s )
	{
	case Side::Top:
	case Side::Bot:
		return( RectI{ pos,dim.x,dim.y } );
	case Side::Left:
	case Side::Right:
		return( RectI{ pos,dim.y,dim.x } );
	default:
		assert( false );
		return( RectI{ -1,-1,-1,-1 } );
		break;
	}
}

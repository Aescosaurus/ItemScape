#include "FloorLevel.h"
#include <vector>
#include "Random.h"

FloorLevel::FloorLevel()
{
	RandomizeLayout();
	// This is for testing all the rooms out. :)
	floorLayout[7] = 5;
}

void FloorLevel::DrawOverlay( Graphics& gfx ) const
{
	RectI room = { 0,roomSize.x,0,roomSize.y };

	room.MoveTo( overlayStart );
	gfx.DrawHitbox( room,CheckRoomColor( 0,0 ) );
	room.MoveBy( roomSize.X() + roomPadding.X() );
	gfx.DrawHitbox( room,CheckRoomColor( 1,0 ) );
	room.MoveBy( roomSize.X() + roomPadding.X() );
	gfx.DrawHitbox( room,CheckRoomColor( 2,0 ) );

	room.MoveTo( overlayStart + roomSize.Y() + roomPadding.Y() );
	gfx.DrawHitbox( room,CheckRoomColor( 0,1 ) );
	room.MoveBy( roomSize.X() + roomPadding.X() );
	gfx.DrawHitbox( room,CheckRoomColor( 1,1 ) );
	room.MoveBy( roomSize.X() + roomPadding.X() );
	gfx.DrawHitbox( room,CheckRoomColor( 2,1 ) );

	room.MoveTo( overlayStart + ( roomSize.Y() + roomPadding.Y() ) * 2 );
	gfx.DrawHitbox( room,CheckRoomColor( 0,2 ) );
	room.MoveBy( roomSize.X() + roomPadding.X() );
	gfx.DrawHitbox( room,CheckRoomColor( 1,2 ) );
	room.MoveBy( roomSize.X() + roomPadding.X() );
	gfx.DrawHitbox( room,CheckRoomColor( 2,2 ) );
}

void FloorLevel::AdvanceFloor()
{
	++curFloor;
	curRoom = { 1,2 };
}

void FloorLevel::MoveRoom( Dir d )
{
	switch( d )
	{
	case Dir::Up:
		--curRoom.y;
		break;
	case Dir::Down:
		++curRoom.y;
		break;
	case Dir::Left:
		--curRoom.x;
		break;
	case Dir::Right:
		++curRoom.x;
		break;
	}
}

std::string FloorLevel::GetLevelName()
{
	return( FormLevelName( floorLayout[curRoom.y * width +
		curRoom.x] ) );
}

int FloorLevel::GetLevelAt( const Vei2& pos ) const
{
	if( pos.x < 0 || pos.x >= width ||
		pos.y < 0 || pos.y >= height )
	{
		return( -1 );
	}

	const auto index = pos.y * width + pos.x;
	return( floorLayout[index] );
}

const Vei2& FloorLevel::GetCurRoom() const
{
	return( curRoom );
}

void FloorLevel::RandomizeLayout()
{
	const int nElements = width * height;
	std::vector<int> layoutNums;
	layoutNums.reserve( nElements + 1 );
	for( int i = 0; i < nElements; ++i )
	{
		layoutNums.emplace_back( i );
	}

	// Using my own shuffle func cuz the std one is bad.
	const int nShuffles = 10;
	for( int i = 0; i < nShuffles; ++i )
	{
		std::swap( layoutNums[Random::RangeI( 0,nElements - 1 )],
			layoutNums[Random::RangeI( 0,nElements - 1 )] );
	}
	
	int bossPos;
	for( int i = 0; i < nElements; ++i )
	{
		floorLayout[i] = layoutNums[i];
		if( layoutNums[i] == 8 ) bossPos = i;
	}

	// Set boss room at right position.
	const int upLeft = 0;
	const int upRight = 2;
	const int idealBossPos = Random::RangeI( 0,10 ) > 5
		? upLeft : upRight;
	// Move boss pos to where it should be.
	std::swap( floorLayout[bossPos],
		floorLayout[idealBossPos] );
}

std::string FloorLevel::FormLevelName( int pos ) const
{
	return( firstName + std::to_string( curFloor ) + "/" +
		levelNames[pos] );
}

Color FloorLevel::CheckRoomColor( int x,int y ) const
{
	if( x == curRoom.x && y == curRoom.y )
	{
		return( Colors::LightGray );
	}
	else
	{
		return( Colors::Gray );
	}
}

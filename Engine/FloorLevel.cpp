#include "FloorLevel.h"
#include <vector>
#include "Random.h"
#include <algorithm>
#include "Utils.h"

FloorLevel::FloorLevel()
{
	RandomizeLayout();
	// This is for testing all the rooms out. :)
	// floorLayout[7] = 8;
}

void FloorLevel::DrawOverlay( Graphics& gfx ) const
{
	RectI room = { 0,roomSize.x,0,roomSize.y };

	room.MoveTo( overlayStart );
	for( int y = 0; y < height; ++y )
	{
		for( int x = 0; x < width; ++x )
		{
			const auto roomCol = CheckRoomColor( x,y );
			if( roomCol != Colors::Magenta )
			{
				gfx.DrawHitbox( room,roomCol );
			}
			room.MoveBy( roomSize.X() + roomPadding.X() );
		}
		room.MoveTo( overlayStart + roomSize.Y() * y +
			roomPadding.Y() * y );
	}

	// room.MoveTo( overlayStart );
	// gfx.DrawHitbox( room,CheckRoomColor( 0,0 ) );
	// room.MoveBy( roomSize.X() + roomPadding.X() );
	// gfx.DrawHitbox( room,CheckRoomColor( 1,0 ) );
	// room.MoveBy( roomSize.X() + roomPadding.X() );
	// gfx.DrawHitbox( room,CheckRoomColor( 2,0 ) );
	// 
	// room.MoveTo( overlayStart + roomSize.Y() + roomPadding.Y() );
	// gfx.DrawHitbox( room,CheckRoomColor( 0,1 ) );
	// room.MoveBy( roomSize.X() + roomPadding.X() );
	// gfx.DrawHitbox( room,CheckRoomColor( 1,1 ) );
	// room.MoveBy( roomSize.X() + roomPadding.X() );
	// gfx.DrawHitbox( room,CheckRoomColor( 2,1 ) );
	// 
	// room.MoveTo( overlayStart + ( roomSize.Y() + roomPadding.Y() ) * 2 );
	// gfx.DrawHitbox( room,CheckRoomColor( 0,2 ) );
	// room.MoveBy( roomSize.X() + roomPadding.X() );
	// gfx.DrawHitbox( room,CheckRoomColor( 1,2 ) );
	// room.MoveBy( roomSize.X() + roomPadding.X() );
	// gfx.DrawHitbox( room,CheckRoomColor( 2,2 ) );
}

void FloorLevel::AdvanceFloor()
{
	++curFloor;
	RandomizeLayout();
	// curRoom = { 1,2 };
	ClearCompletedRooms();
}

void FloorLevel::MoveRoom( Dir d )
{
	if( !CurRoomAlreadyCompleted() )
	{
		completedRooms.emplace_back( curRoom );
	}

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

void FloorLevel::ClearCompletedRooms()
{
	completedRooms.clear();
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

bool FloorLevel::CurRoomAlreadyCompleted() const
{
	for( const auto& vec : completedRooms )
	{
		if( vec == curRoom )
		{
			return( true );
		}
	}

	return( false );
}

void FloorLevel::RandomizeLayout()
{
	// Reset all tiles to blank tiles.
	for( int i = 0; i < width * height; ++i )
	{
		floorLayout[i] = -1;
	}

	// const int nElements = width * height;
	std::vector<int> layoutNums;
	layoutNums.reserve( nRoomsPerFloor + 1 );
	for( int i = 0; i < nRoomsPerFloor - 1; ++i )
	{
		layoutNums.emplace_back( i );
	}

	// Shuffle order of layout nums.
	std::random_device rd;
	std::mt19937 rng{ rd() };
	std::shuffle( layoutNums.begin(),layoutNums.end(),rng );

	layoutNums.insert( layoutNums.begin(),8 );

	// // Get boss pos in start of vector.
	// for( int i = 0; i < nRoomsPerFloor; ++i )
	// {
	// 	if( layoutNums[i] == 8 )
	// 	{
	// 		std::swap( layoutNums[0],layoutNums[i] );
	// 		break;
	// 	}
	// }

	// Place the actual rooms in a random layout.
	Vei2 start = { Random::RangeI( 1,width - 1 ),
		Random::RangeI( 1,height - 1 ) };
	while( layoutNums.size() > 0 )
	{
		if( floorLayout[start.y * width + start.x] == -1 )
		{
			floorLayout[start.y * width + start.x] = layoutNums[0];

			layoutNums.erase( layoutNums.begin() );
		}

		if( layoutNums.size() > 0 )
		{
			const int randMoveDir = Random::RangeI( 0,3 );
			switch( randMoveDir )
			{
			case 0:
				if( start.y > 1 ) --start.y;
				break;
			case 1:
				if( start.y < height - 1 ) ++start.y;
				break;
			case 2:
				if( start.x > 0 ) --start.x;
				break;
			case 3:
				if( start.x < width - 1 ) ++start.x;
				break;
			}
		}
	}

	curRoom = start;
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
	else if( floorLayout[y * width + x] == 8 )
	{
		return( Colors::Red );
	}
	else if( floorLayout[y * width + x] == -1 )
	{
		return( Colors::Magenta );
	}
	else
	{
		return( Colors::Gray );
	}
}

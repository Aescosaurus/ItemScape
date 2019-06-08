#pragma once

#include <string>
#include "Vec2.h"
#include "Graphics.h"

// Floors are 3x3 collections of rooms.
// Player always spawns in lower middle room.
// Boss always spawns in upper left or upper right room.
// Levels 0-7 are randomized between all the places.
class FloorLevel
{
public:
	enum class Dir
	{
		Up,
		Down,
		Left,
		Right
	};
public:
	FloorLevel();

	void DrawOverlay( Graphics& gfx ) const;

	void AdvanceFloor();
	void MoveRoom( Dir d );
	void ClearCompletedRooms();
	void LoadSaveInfo( const std::string& info );

	std::string GetLevelName();
	int GetLevelAt( const Vei2& pos ) const;
	const Vei2& GetCurRoom() const;
	bool CurRoomAlreadyCompleted() const;
	std::string GenerateSaveInfo() const;
private:
	void RandomizeLayout();

	std::string FormLevelName( int pos ) const;
	Color CheckRoomColor( int x,int y ) const;
private:
	// static constexpr int width = 3;
	// static constexpr int height = 3;
	// int floorLayout[width * height] =
	// { // This is what it should look like but shuffled.
	// 	0,1,2, // Oh, and 8 should only be on upper left
	// 	3,4,5, //  or upper right.
	// 	6,7,8
	// };
	static constexpr int width = 9;
	static constexpr int height = 9;
	static constexpr int nRoomsPerFloor = 9;
	int floorLayout[width * height];

	int curFloor = 0;
	static constexpr auto firstName = "Levels/Floor";
	Vei2 curRoom = { 1,2 };

	static constexpr char* levelNames[] =
	{
		"Level0.lvl",
		"Level1.lvl",
		"Level2.lvl",
		"Level3.lvl",
		"Level4.lvl",
		"Level5.lvl",
		"Level6.lvl",
		"Level7.lvl",
		"BossRoom.lvl"
	};

	std::vector<Vei2> completedRooms;

	static constexpr Vei2 overlayStart = { 50,50 };
	static constexpr Vei2 roomSize = { 35 * 2,20 * 2 };
	static constexpr Vei2 roomPadding = { 5,5 };
};
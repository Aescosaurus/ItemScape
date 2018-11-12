#pragma once

#include <string>
#include "Vec2.h"

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

	void AdvanceFloor();
	void MoveRoom( Dir d );

	std::string GetLevelName();
	int GetLevelAt( const Vei2& pos ) const;
	const Vei2& GetCurRoom() const;
private:
	void RandomizeLayout();

	std::string FormLevelName( int pos ) const;
private:
	static constexpr int width = 3;
	static constexpr int height = 3;
	int floorLayout[width * height] =
	{ // This is what it should look like but shuffled.
		0,1,2, // Oh, and 8 should only be on upper left
		3,4,5, //  or upper right.
		6,7,8
	};

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
};
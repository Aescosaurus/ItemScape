#pragma once

#include <vector>
#include <string>
#include "Graphics.h"
#include <memory>
#include "Surface.h"

class DataPoint
{
public:
	DataPoint() = delete;
	DataPoint( const Vei2& pos,char type )
		:
		pos( pos ),
		type( type )
	{}
public:
	Vei2 pos;
	char type;
};

class TileMap
{
public:
	enum class TileType
	{
		Empty = int( '0' ),
		Wall = int( '1' )
	};
public:
	TileMap() = default;

	void Draw( Graphics& gfx ) const;
	// If you ever end up adding a background or floor
	//  tiles, make sure to draw them here.
	void DrawFloor( Graphics& gfx ) const;
	void DrawTileAt( int x,int y,Color c,Graphics& gfx ) const;

	void SetTile( int x,int y,TileType type );
	void LoadFile( const std::string& fileName );
	std::vector<Vec2> FindAllInstances( char searchTerm,
		const std::string& fileName );
	void AdvanceFloor();

	TileType GetTile( int x,int y ) const;
	Vei2 GetTilePos( const Vec2& screenPos ) const;
	TileType GetTileAt( const Vec2& screenPos ) const;
	std::vector<DataPoint> FindSpecialTerms( const std::string& fileName ) const;
private:
	int width; // Number of tiles left-right.
	int height; // Number of tiles up-down.
	Vei2 tileDim; // Size of each tile on screen.
	std::vector<TileType> tiles;

	static constexpr int nWallTypes = 2;
	Surface wallSprs[nWallTypes] =
	{
		"Images/Wall1.bmp",
		"Images/Wall2.bmp"
	};
	Surface wallTops[nWallTypes] =
	{
		"Images/Wall1Top.bmp",
		"Images/Wall2Top.bmp"
	};
	Surface floorSprs[nWallTypes] =
	{
		"Images/Floor1.bmp",
		"Images/Floor1.bmp"
	};
	Surface floorCloses[nWallTypes] =
	{
		"Images/Floor1Close.bmp",
		"Images/Floor1Close.bmp"
	};
	int curWallIndex = 0;
};
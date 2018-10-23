#include "TileMap.h"
#include <fstream>
#include <cassert>

TileMap::TileMap( const std::string& fileName )
{
	LoadFile( fileName );
}

void TileMap::Draw( Graphics& gfx ) const
{
	for( int y = 0; y < height; ++y )
	{
		for( int x = 0; x < width; ++x )
		{
			const auto tile = GetTile( x,y );
			if( tile == TileType::Wall )
			{
				gfx.DrawRect( x * tileDim.x,y * tileDim.y,
					tileDim.x,tileDim.y,
					Colors::Gray );
			}
		}
	}
}

void TileMap::DrawTileAt( int x,int y,Color c,Graphics& gfx ) const
{
	// gfx.DrawRect( x * tileDim.x,y * tileDim.y,
	// 	tileDim.x,tileDim.y,
	// 	c );
	gfx.DrawHitbox( Rect( RectI( { x * tileDim.x,y * tileDim.y },
		tileDim.x,tileDim.y ) ),c );
}

void TileMap::SetTile( int x,int y,TileType type )
{
	tiles[y * width + x] = type;
}

void TileMap::LoadFile( const std::string& fileName )
{
	std::ifstream in( fileName );
	assert( in.good() );

	tiles.clear();
	int tempWidth = 0;
	bool done = false;
	int tempHeight = 1; // cuz theres no newline at eof.

	for( char c = in.get(); in.good(); c = in.get() )
	{
		if( c == '\n' )
		{
			done = true;
			++tempHeight;
		}
		else if( c != ',' )
		{
			tiles.emplace_back( TileType( int( c - '0' ) ) );
			if( !done ) ++tempWidth;
		}
	}

	width = tempWidth;
	height = tempHeight;

	tileDim.x = Graphics::ScreenWidth / width;
	tileDim.y = Graphics::ScreenHeight / height;
}

TileMap::TileType TileMap::GetTile( int x,int y ) const
{
	return( tiles[y * width + x] );
}

Vei2 TileMap::GetTilePos( const Vec2& screenPos ) const
{
	const auto x = int( std::floor( screenPos.x ) /
		( float( Graphics::ScreenWidth ) / width ) );
	const auto y = int( std::floor( screenPos.y ) /
		( float( Graphics::ScreenHeight ) / height ) );

	return( Vei2{ x,y } );
}

TileMap::TileType TileMap::GetTileAt( const Vec2& screenPos ) const
{
	const auto pos = GetTilePos( screenPos );
	return( GetTile( pos.x,pos.y ) );
}

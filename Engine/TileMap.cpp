#include "TileMap.h"
#include <fstream>
#include <cassert>
#include "EnemyType.h"

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
			TileType toAdd;
			switch( c )
			{
			case int( TileType::Wall ):
				toAdd = TileType::Wall;
				break;
			case int( TileType::Empty ):
				toAdd = TileType::Empty;
				break;
			default:
				// assert( false );
				break;
			}
			tiles.emplace_back( toAdd );
			if( !done ) ++tempWidth;
		}
	}

	width = tempWidth;
	height = tempHeight;

	tileDim.x = Graphics::ScreenWidth / width;
	tileDim.y = Graphics::ScreenHeight / height;
}

std::vector<Vec2> TileMap::FindAllInstances( char searchTerm,
	const std::string& fileName )
{
	std::vector<Vec2> results;
	std::ifstream in( fileName );
	assert( in.good() );

	int x = 0;
	int y = 0;
	for( char c = in.get(); in.good(); c = in.get() )
	{
		if( c == ',' ) continue;

		if( c == searchTerm )
		{
			results.emplace_back( Vei2( x * tileDim.x - tileDim.x,
				y * tileDim.y ) );
		}
		if( c == '\n' )
		{
			x = 0;
			++y;
		}

		++x;
	}

	return( results );
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

std::vector<DataPoint> TileMap::FindSpecialTerms( const std::string& fileName ) const
{
	std::vector<DataPoint> results;
	std::ifstream in( fileName );

	Vei2 pos = { 0,0 };
	for( char c = in.get(); in.good(); c = in.get() )
	{
		switch( c )
		{
		case '\n':
			++pos.y;
			pos.x = 0;
			break;
		case ',':
			++pos.x;
			break;
		case '0':
		case '1':
			break;
		default:
			results.emplace_back( DataPoint{ Vei2{ pos.x *
				tileDim.x,pos.y * tileDim.y },c } );
			break;
		}
	}

	return( results );
}

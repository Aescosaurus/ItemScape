#include "SaveLoader.h"
#include <fstream>

void SaveLoader::Save( const SaveLoaderInfo& info,int slot )
{
	std::ofstream out{ fileName + std::to_string( slot ) + ".txt" };
	assert( out.good() );

	out << info.floor.GenerateSaveInfo();
}

void SaveLoader::Load( SaveLoaderInfo& info,int slot )
{
	std::ifstream in{ fileName + std::to_string( slot ) + ".txt" };
	assert( in.good() );

	std::string floorInfo = "";

	int i = 0;
	for( char c = in.get(); in.good() && i < 4;
		c = in.get() )
	{
		floorInfo += c;
		if( c == '\n' ) ++i;
	}

	info.floor.LoadSaveInfo( floorInfo );

	// inv stuff
}

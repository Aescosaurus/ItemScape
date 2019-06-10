#include "SaveLoader.h"
#include <fstream>

void SaveLoader::Save( const SaveLoaderInfo& info,int slot )
{
	std::ofstream out{ fileName + std::to_string( slot ) + ".txt" };
	assert( out.good() );

	out << info.floor.GenerateSaveInfo();
	out << info.inv.GenerateSaveInfo();
}

void SaveLoader::Load( SaveLoaderInfo& info,int slot )
{
	std::ifstream in{ fileName + std::to_string( slot ) + ".txt" };
	assert( in.good() );

	std::string floorInfo = "";

	int i = 0;
	for( char c = in.get(); i < 4 && in.good(); )
	{
		floorInfo += c;
		if( c == '\n' ) ++i;
		if( i < 4 ) c = in.get();
	}

	info.floor.LoadSaveInfo( floorInfo );

	std::string invInfo = "";
	for( char c = in.get(); in.good(); c = in.get() )
	{
		invInfo += c;
	}

	info.inv.LoadSaveInfo( invInfo );
}

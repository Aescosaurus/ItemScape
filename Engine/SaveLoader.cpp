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
}

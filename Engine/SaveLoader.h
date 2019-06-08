#pragma once

#include "FloorLevel.h"
#include "Inventory.h"

class SaveLoaderInfo
{
public:
	FloorLevel& floor;
	Inventory& inv;
};

class SaveLoader
{
public:
	static void Save( const SaveLoaderInfo& info,int slot );
	static void Load( SaveLoaderInfo& info,int slot );
private:
	static constexpr auto fileName = "Saves/Save";
};
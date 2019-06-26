#pragma once

#include "InventoryItem.h"

class MithrilHoe
	:
	public InventoryItem
{
public:
	MithrilHoe()
		:
		InventoryItem( "ItemDescriptions/MithrilHoe.txt",
			"Images/Items/MithrilHoe.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new MithrilHoe{ *this } );
	}

	// TODO: Add txt and bmp.
	// TODO: %chance to stick enemies.
};
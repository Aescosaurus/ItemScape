#pragma once

#include "InventoryItem.h"

class CactusBranch
	:
	public InventoryItem
{
public:
	CactusBranch()
		:
		InventoryItem( "ItemDescriptions/CactusBranch.txt",
			"Images/Items/CactusBranch.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new CactusBranch{ *this } );
	}

	void OnPlayerHit( InventoryEventInfo& invEvtInfo ) override
	{
		if( invEvtInfo.enemies.size() > 0 )
		{
			invEvtInfo.enemies[Random::RangeI(
				0,int( invEvtInfo.enemies.size() ) - 1 )]
				->Attack( damage,invEvtInfo.player.GetPos() );
		}
	}
private:
	static constexpr int damage = 20;
};
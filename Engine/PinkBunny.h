#pragma once

#include "InventoryItem.h"

class PinkBunny
	:
	public InventoryItem
{
public:
	PinkBunny()
		:
		InventoryItem( "ItemDescriptions/PinkBunny.txt",
			"Images/Wall2.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new PinkBunny );
	}

	void OnActivate( InventoryEventInfo& evtInfo ) override
	{
		evtInfo.items[0]->BoostFireRate( evtInfo,
			fireRateBuff,0,shotSpeedupDuration );

		remove = true;
	}
private:
	static constexpr float fireRateBuff = 5.5f;
	static constexpr float shotSpeedupDuration = 15.0f;
};
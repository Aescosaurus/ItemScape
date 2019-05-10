#pragma once

#include "InventoryItem.h"

class HealthCharge
	:
	public InventoryItem
{
public:
	HealthCharge()
		:
		InventoryItem( "ItemDescriptions/HealthCharge.txt",
			"Images/Items/HealthCharge.bmp" )
	{}

	void OnPlayerHit( InventoryEventInfo& evtInfo ) override
	{
		evtInfo.enemyBullets.clear();

		remove = true;
	}
};
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
			"Images/Items/PinkBunny.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new PinkBunny );
	}

	void OnActivate( InventoryEventInfo& evtInfo ) override
	{
		if( !activated )
		{
			evtInfo.items[0]->BoostFireRate( evtInfo,
				fireRateBuff );

			activated = true;
		}
	}

	void OnUpdate( InventoryEventInfo& evtInfo ) override
	{
		if( activated )
		{
			shotSpeedupDuration.Update( evtInfo.dt );

			if( shotSpeedupDuration.IsDone() &&
				evtInfo.items[0]->IsGun() )
			{
				evtInfo.items[0]->RemoveFireRateBoost( evtInfo,
					fireRateBuff );

				remove = true;
			}
		}
	}
private:
	static constexpr float fireRateBuff = 1.5f;
	Timer shotSpeedupDuration = 10.0f;
	bool activated = false;
};
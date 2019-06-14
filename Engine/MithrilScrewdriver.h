#pragma once

#include "InventoryItem.h"
#include "GunBase.h"

class MithrilScrewdriver
	:
	public InventoryItem
{
public:
	MithrilScrewdriver()
		:
		InventoryItem( "ItemDescriptions/MithrilScrewdriver.txt",
			"Images/Items/MithrilScrewdriver.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new MithrilScrewdriver{ *this } );
	}

	void OnRoomStart( InventoryEventInfo& evtInfo ) override
	{
		if( !boostStarted )
		{
			GunBase::AddFireRateBoost( evtInfo,fireRateBoost );
			boostStarted = true;
			boostDuration.Reset();
		}
		else
		{
			boostStarted = false;
			GunBase::RemoveFireRateBoost( evtInfo,fireRateBoost );
		}
	}

	void OnUpdate( InventoryEventInfo& evtInfo ) override
	{
		if( boostStarted )
		{
			boostDuration.Update( evtInfo.dt );

			if( boostDuration.IsDone() )
			{
				boostStarted = false;
				GunBase::RemoveFireRateBoost( evtInfo,fireRateBoost );
			}
		}
	}
private:
	static constexpr float fireRateBoost = 1.2f;
	Timer boostDuration = 10.0f;
	bool boostStarted = false;
};
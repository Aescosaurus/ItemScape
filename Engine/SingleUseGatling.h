#pragma once

#include "InventoryItem.h"

class SingleUseGatling
	:
	public InventoryItem
{
public:
	SingleUseGatling()
		:
		InventoryItem( "ItemDescriptions/SingleUseGatling.txt",
			"Images/Items/SingleUseGatling.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new SingleUseGatling );
	}

	void OnActivate( InventoryEventInfo& invEvtInfo ) override
	{
		activated = true;
	}

	void OnUpdate( InventoryEventInfo& invEvtInfo ) override
	{
		if( activated )
		{
			refire.Update( invEvtInfo.dt );
		
			if( refire.IsDone() )
			{
				invEvtInfo.items[0]->Shoot( invEvtInfo,
					Vec2( invEvtInfo.mouse.GetPos() ) );

				refire.Reset();
				++curShot;
				
				if( curShot >= nShots )
				{
					remove = true;
				}
			}
		}
	}
private:
	static constexpr int nShots = 10;
	int curShot = 0;
	Timer refire = 0.1f;
	bool activated = false;
};
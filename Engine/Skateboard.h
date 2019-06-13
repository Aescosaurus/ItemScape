#pragma once

#include "InventoryItem.h"

class Skateboard
	:
	public InventoryItem
{
public:
	Skateboard()
		:
		InventoryItem( "ItemDescriptions/Skateboard.txt",
			"Images/Items/Skateboard.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new Skateboard{ *this } );
	}

	void OnUpdate( InventoryEventInfo& evtInfo ) override
	{
		timeTillBoost.Update( evtInfo.dt );

		if( timeTillBoost.IsDone() )
		{
			if( !boostActive )
			{
				boostActive = true;
				evtInfo.player.MultiplyMoveSpeedFactor( moveSpeedBoost );
			}
		}
	}

	void OnPlayerShoot( InventoryEventInfo& evtInfo ) override
	{
		timeTillBoost.Reset();
		if( boostActive )
		{
			boostActive = false;
			evtInfo.player.MultiplyMoveSpeedFactor( 1.0f / moveSpeedBoost );
		}
	}
private:
	static constexpr float moveSpeedBoost = 1.25f;
	Timer timeTillBoost = 0.5f;
	bool boostActive = false;
};
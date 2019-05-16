#pragma once

#include "InventoryItem.h"

class UggsOfMobility
	:
	public InventoryItem
{
public:
	UggsOfMobility()
		:
		InventoryItem( "ItemDescriptions/UggsOfMobility.txt",
			"Images/Items/UggsOfMobility.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new UggsOfMobility );
	}

	void OnReceive( InventoryEventInfo& evtInfo ) override
	{
		evtInfo.player.MultiplyMoveSpeedFactor( moveSpeedMult );
	}

	void OnRemove( InventoryEventInfo& evtInfo ) override
	{
		evtInfo.player.MultiplyMoveSpeedFactor( 1.0f / moveSpeedMult );
	}
private:
	static constexpr float moveSpeedMult = 1.15f;
};